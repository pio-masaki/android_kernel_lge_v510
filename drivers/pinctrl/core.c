/*
 * Core driver for the pin control subsystem
 *
 * Copyright (C) 2011-2012 ST-Ericsson SA
 * Written on behalf of Linaro for ST-Ericsson
 * Based on bits of regulator core, gpio core and clk core
 *
 * Author: Linus Walleij <linus.walleij@linaro.org>
 *
 * Copyright (C) 2012 NVIDIA CORPORATION. All rights reserved.
 *
 * License terms: GNU General Public License (GPL) version 2
 */
#define pr_fmt(fmt) "pinctrl core: " fmt

#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/sysfs.h>
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/machine.h>
#include "core.h"
#include "pinmux.h"
#include "pinconf.h"

/* 
                                                                         
                                 
                                        
                                            
 */
struct pinctrl_maps {
	struct list_head node;
	struct pinctrl_map const *maps;
	unsigned num_maps;
};

/*                                 */
DEFINE_MUTEX(pinctrl_mutex);

/*                                                         */
static LIST_HEAD(pinctrldev_list);

/*                                                 */
static LIST_HEAD(pinctrl_list);

/*                                            */
static LIST_HEAD(pinctrl_maps);

#define for_each_maps(_maps_node_, _i_, _map_) \
	list_for_each_entry(_maps_node_, &pinctrl_maps, node) \
		for (_i_ = 0, _map_ = &_maps_node_->maps[_i_]; \
			_i_ < _maps_node_->num_maps; \
			i++, _map_ = &_maps_node_->maps[_i_])

const char *pinctrl_dev_get_name(struct pinctrl_dev *pctldev)
{
	/*                                                    */
	return pctldev->desc->name;
}
EXPORT_SYMBOL_GPL(pinctrl_dev_get_name);

void *pinctrl_dev_get_drvdata(struct pinctrl_dev *pctldev)
{
	return pctldev->driver_data;
}
EXPORT_SYMBOL_GPL(pinctrl_dev_get_drvdata);

/* 
                                                                 
                                                                     
  
                                                                              
                                                         
 */
struct pinctrl_dev *get_pinctrl_dev_from_devname(const char *devname)
{
	struct pinctrl_dev *pctldev = NULL;
	bool found = false;

	if (!devname)
		return NULL;

	list_for_each_entry(pctldev, &pinctrldev_list, node) {
		if (!strcmp(dev_name(pctldev->dev), devname)) {
			/*                        */
			found = true;
			break;
		}
	}

	return found ? pctldev : NULL;
}

/* 
                                                         
                                                        
                                        
 */
int pin_get_from_name(struct pinctrl_dev *pctldev, const char *name)
{
	unsigned i, pin;

	/*                                                                   */
	for (i = 0; i < pctldev->desc->npins; i++) {
		struct pin_desc *desc;

		pin = pctldev->desc->pins[i].number;
		desc = pin_desc_get(pctldev, pin);
		/*                         */
		if (desc == NULL)
			continue;
		if (desc->name && !strcmp(name, desc->name))
			return pin;
	}

	return -EINVAL;
}

/* 
                                                     
                                                       
                                                                
  
                                                                           
                                                            
 */
bool pin_is_valid(struct pinctrl_dev *pctldev, int pin)
{
	struct pin_desc *pindesc;

	if (pin < 0)
		return false;

	mutex_lock(&pinctrl_mutex);
	pindesc = pin_desc_get(pctldev, pin);
	mutex_unlock(&pinctrl_mutex);

	return pindesc != NULL;
}
EXPORT_SYMBOL_GPL(pin_is_valid);

/*                                    */
static void pinctrl_free_pindescs(struct pinctrl_dev *pctldev,
				  const struct pinctrl_pin_desc *pins,
				  unsigned num_pins)
{
	int i;

	for (i = 0; i < num_pins; i++) {
		struct pin_desc *pindesc;

		pindesc = radix_tree_lookup(&pctldev->pin_desc_tree,
					    pins[i].number);
		if (pindesc != NULL) {
			radix_tree_delete(&pctldev->pin_desc_tree,
					  pins[i].number);
			if (pindesc->dynamic_name)
				kfree(pindesc->name);
		}
		kfree(pindesc);
	}
}

static int pinctrl_register_one_pin(struct pinctrl_dev *pctldev,
				    unsigned number, const char *name)
{
	struct pin_desc *pindesc;

	pindesc = pin_desc_get(pctldev, number);
	if (pindesc != NULL) {
		pr_err("pin %d already registered on %s\n", number,
		       pctldev->desc->name);
		return -EINVAL;
	}

	pindesc = kzalloc(sizeof(*pindesc), GFP_KERNEL);
	if (pindesc == NULL) {
		dev_err(pctldev->dev, "failed to alloc struct pin_desc\n");
		return -ENOMEM;
	}

	/*           */
	pindesc->pctldev = pctldev;

	/*                     */
	if (name) {
		pindesc->name = name;
	} else {
		pindesc->name = kasprintf(GFP_KERNEL, "PIN%u", number);
		if (pindesc->name == NULL)
			return -ENOMEM;
		pindesc->dynamic_name = true;
	}

	radix_tree_insert(&pctldev->pin_desc_tree, number, pindesc);
	pr_debug("registered pin %d (%s) on %s\n",
		 number, pindesc->name, pctldev->desc->name);
	return 0;
}

static int pinctrl_register_pins(struct pinctrl_dev *pctldev,
				 struct pinctrl_pin_desc const *pins,
				 unsigned num_descs)
{
	unsigned i;
	int ret = 0;

	for (i = 0; i < num_descs; i++) {
		ret = pinctrl_register_one_pin(pctldev,
					       pins[i].number, pins[i].name);
		if (ret)
			return ret;
	}

	return 0;
}

/* 
                                                                       
                                           
                                                                
  
                                                                          
                                       
 */
static struct pinctrl_gpio_range *
pinctrl_match_gpio_range(struct pinctrl_dev *pctldev, unsigned gpio)
{
	struct pinctrl_gpio_range *range = NULL;

	/*                      */
	list_for_each_entry(range, &pctldev->gpio_ranges, node) {
		/*                                   */
		if (gpio >= range->base &&
		    gpio < range->base + range->npins) {
			return range;
		}
	}

	return NULL;
}

/* 
                                                               
                                                  
                                           
                                     
  
                                                                           
                                                                             
                                                               
 */
static int pinctrl_get_device_gpio_range(unsigned gpio,
					 struct pinctrl_dev **outdev,
					 struct pinctrl_gpio_range **outrange)
{
	struct pinctrl_dev *pctldev = NULL;

	/*                               */
	list_for_each_entry(pctldev, &pinctrldev_list, node) {
		struct pinctrl_gpio_range *range;

		range = pinctrl_match_gpio_range(pctldev, gpio);
		if (range != NULL) {
			*outdev = pctldev;
			*outrange = range;
			return 0;
		}
	}

	return -EINVAL;
}

/* 
                                                                    
                                                      
                                
  
                                                                             
                                                                         
 */
void pinctrl_add_gpio_range(struct pinctrl_dev *pctldev,
			    struct pinctrl_gpio_range *range)
{
	mutex_lock(&pinctrl_mutex);
	list_add_tail(&range->node, &pctldev->gpio_ranges);
	mutex_unlock(&pinctrl_mutex);
}
EXPORT_SYMBOL_GPL(pinctrl_add_gpio_range);

/* 
                                                                             
                                                           
                                   
 */
void pinctrl_remove_gpio_range(struct pinctrl_dev *pctldev,
			       struct pinctrl_gpio_range *range)
{
	mutex_lock(&pinctrl_mutex);
	list_del(&range->node);
	mutex_unlock(&pinctrl_mutex);
}
EXPORT_SYMBOL_GPL(pinctrl_remove_gpio_range);

/* 
                                                                        
                                                  
                                       
 */
int pinctrl_get_group_selector(struct pinctrl_dev *pctldev,
			       const char *pin_group)
{
	const struct pinctrl_ops *pctlops = pctldev->desc->pctlops;
	unsigned group_selector = 0;

	while (pctlops->list_groups(pctldev, group_selector) >= 0) {
		const char *gname = pctlops->get_group_name(pctldev,
							    group_selector);
		if (!strcmp(gname, pin_group)) {
			dev_dbg(pctldev->dev,
				"found group selector %u for %s\n",
				group_selector,
				pin_group);
			return group_selector;
		}

		group_selector++;
	}

	dev_err(pctldev->dev, "does not have pin group %s\n",
		pin_group);

	return -EINVAL;
}

/* 
                                                                      
                                                                  
  
                                                                       
                                                                              
                                                
 */
int pinctrl_request_gpio(unsigned gpio)
{
	struct pinctrl_dev *pctldev;
	struct pinctrl_gpio_range *range;
	int ret;
	int pin;

	mutex_lock(&pinctrl_mutex);

	ret = pinctrl_get_device_gpio_range(gpio, &pctldev, &range);
	if (ret) {
		mutex_unlock(&pinctrl_mutex);
		return -EINVAL;
	}

	/*                                             */
	pin = gpio - range->base + range->pin_base;

	ret = pinmux_request_gpio(pctldev, range, pin, gpio);

	mutex_unlock(&pinctrl_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(pinctrl_request_gpio);

/* 
                                                                             
                                                                  
  
                                                                       
                                                                           
                                                 
 */
void pinctrl_free_gpio(unsigned gpio)
{
	struct pinctrl_dev *pctldev;
	struct pinctrl_gpio_range *range;
	int ret;
	int pin;

	mutex_lock(&pinctrl_mutex);

	ret = pinctrl_get_device_gpio_range(gpio, &pctldev, &range);
	if (ret) {
		mutex_unlock(&pinctrl_mutex);
		return;
	}

	/*                                             */
	pin = gpio - range->base + range->pin_base;

	pinmux_free_gpio(pctldev, pin, range);

	mutex_unlock(&pinctrl_mutex);
}
EXPORT_SYMBOL_GPL(pinctrl_free_gpio);

static int pinctrl_gpio_direction(unsigned gpio, bool input)
{
	struct pinctrl_dev *pctldev;
	struct pinctrl_gpio_range *range;
	int ret;
	int pin;

	ret = pinctrl_get_device_gpio_range(gpio, &pctldev, &range);
	if (ret)
		return ret;

	/*                                             */
	pin = gpio - range->base + range->pin_base;

	return pinmux_gpio_direction(pctldev, range, pin, input);
}

/* 
                                                                            
                                                                  
  
                                                                       
                                                                              
                                                    
 */
int pinctrl_gpio_direction_input(unsigned gpio)
{
	int ret;
	mutex_lock(&pinctrl_mutex);
	ret = pinctrl_gpio_direction(gpio, true);
	mutex_unlock(&pinctrl_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(pinctrl_gpio_direction_input);

/* 
                                                                              
                                                                  
  
                                                                       
                                                                               
                                                    
 */
int pinctrl_gpio_direction_output(unsigned gpio)
{
	int ret;
	mutex_lock(&pinctrl_mutex);
	ret = pinctrl_gpio_direction(gpio, false);
	mutex_unlock(&pinctrl_mutex);
	return ret;
}
EXPORT_SYMBOL_GPL(pinctrl_gpio_direction_output);

static struct pinctrl_state *find_state(struct pinctrl *p,
					const char *name)
{
	struct pinctrl_state *state;

	list_for_each_entry(state, &p->states, node)
		if (!strcmp(state->name, name))
			return state;

	return NULL;
}

static struct pinctrl_state *create_state(struct pinctrl *p,
					  const char *name)
{
	struct pinctrl_state *state;

	state = kzalloc(sizeof(*state), GFP_KERNEL);
	if (state == NULL) {
		dev_err(p->dev,
			"failed to alloc struct pinctrl_state\n");
		return ERR_PTR(-ENOMEM);
	}

	state->name = name;
	INIT_LIST_HEAD(&state->settings);

	list_add_tail(&state->node, &p->states);

	return state;
}

static int add_setting(struct pinctrl *p, struct pinctrl_map const *map)
{
	struct pinctrl_state *state;
	struct pinctrl_setting *setting;
	int ret;

	state = find_state(p, map->name);
	if (!state)
		state = create_state(p, map->name);
	if (IS_ERR(state))
		return PTR_ERR(state);

	if (map->type == PIN_MAP_TYPE_DUMMY_STATE)
		return 0;

	setting = kzalloc(sizeof(*setting), GFP_KERNEL);
	if (setting == NULL) {
		dev_err(p->dev,
			"failed to alloc struct pinctrl_setting\n");
		return -ENOMEM;
	}

	setting->type = map->type;

	setting->pctldev = get_pinctrl_dev_from_devname(map->ctrl_dev_name);
	if (setting->pctldev == NULL) {
		dev_err(p->dev, "unknown pinctrl device %s in map entry",
			map->ctrl_dev_name);
		kfree(setting);
		/*                                                */
		return -ENODEV;
	}

	switch (map->type) {
	case PIN_MAP_TYPE_MUX_GROUP:
		ret = pinmux_map_to_setting(map, setting);
		break;
	case PIN_MAP_TYPE_CONFIGS_PIN:
	case PIN_MAP_TYPE_CONFIGS_GROUP:
		ret = pinconf_map_to_setting(map, setting);
		break;
	default:
		ret = -EINVAL;
		break;
	}
	if (ret < 0) {
		kfree(setting);
		return ret;
	}

	list_add_tail(&setting->node, &state->settings);

	return 0;
}

static struct pinctrl *find_pinctrl(struct device *dev)
{
	struct pinctrl *p;

	list_for_each_entry(p, &pinctrl_list, node)
		if (p->dev == dev)
			return p;

	return NULL;
}

static void pinctrl_put_locked(struct pinctrl *p, bool inlist);

static struct pinctrl *create_pinctrl(struct device *dev)
{
	struct pinctrl *p;
	const char *devname;
	struct pinctrl_maps *maps_node;
	int i;
	struct pinctrl_map const *map;
	int ret;

	/*
                                                          
                                                            
                                           
  */
	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (p == NULL) {
		dev_err(dev, "failed to alloc struct pinctrl\n");
		return ERR_PTR(-ENOMEM);
	}
	p->dev = dev;
	INIT_LIST_HEAD(&p->states);

	devname = dev_name(dev);

	/*                                                            */
	for_each_maps(maps_node, i, map) {
		/*                             */
		if (strcmp(map->dev_name, devname))
			continue;

		ret = add_setting(p, map);
		if (ret < 0) {
			pinctrl_put_locked(p, false);
			return ERR_PTR(ret);
		}
	}

	/*                                   */
	list_add_tail(&p->node, &pinctrl_list);

	return p;
}

static struct pinctrl *pinctrl_get_locked(struct device *dev)
{
	struct pinctrl *p;

	if (WARN_ON(!dev))
		return ERR_PTR(-EINVAL);

	p = find_pinctrl(dev);
	if (p != NULL)
		return ERR_PTR(-EBUSY);

	p = create_pinctrl(dev);
	if (IS_ERR(p))
		return p;

	return p;
}

/* 
                                                            
                                            
 */
struct pinctrl *pinctrl_get(struct device *dev)
{
	struct pinctrl *p;

	mutex_lock(&pinctrl_mutex);
	p = pinctrl_get_locked(dev);
	mutex_unlock(&pinctrl_mutex);

	return p;
}
EXPORT_SYMBOL_GPL(pinctrl_get);

static void pinctrl_put_locked(struct pinctrl *p, bool inlist)
{
	struct pinctrl_state *state, *n1;
	struct pinctrl_setting *setting, *n2;

	list_for_each_entry_safe(state, n1, &p->states, node) {
		list_for_each_entry_safe(setting, n2, &state->settings, node) {
			switch (setting->type) {
			case PIN_MAP_TYPE_MUX_GROUP:
				if (state == p->state)
					pinmux_disable_setting(setting);
				pinmux_free_setting(setting);
				break;
			case PIN_MAP_TYPE_CONFIGS_PIN:
			case PIN_MAP_TYPE_CONFIGS_GROUP:
				pinconf_free_setting(setting);
				break;
			default:
				break;
			}
			list_del(&setting->node);
			kfree(setting);
		}
		list_del(&state->node);
		kfree(state);
	}

	if (inlist)
		list_del(&p->node);
	kfree(p);
}

/* 
                                                              
                                    
 */
void pinctrl_put(struct pinctrl *p)
{
	mutex_lock(&pinctrl_mutex);
	pinctrl_put_locked(p, true);
	mutex_unlock(&pinctrl_mutex);
}
EXPORT_SYMBOL_GPL(pinctrl_put);

static struct pinctrl_state *pinctrl_lookup_state_locked(struct pinctrl *p,
							 const char *name)
{
	struct pinctrl_state *state;

	state = find_state(p, name);
	if (!state)
		return ERR_PTR(-ENODEV);

	return state;
}

/* 
                                                                          
                                                    
                                    
 */
struct pinctrl_state *pinctrl_lookup_state(struct pinctrl *p, const char *name)
{
	struct pinctrl_state *s;

	mutex_lock(&pinctrl_mutex);
	s = pinctrl_lookup_state_locked(p, name);
	mutex_unlock(&pinctrl_mutex);

	return s;
}
EXPORT_SYMBOL_GPL(pinctrl_lookup_state);

static int pinctrl_select_state_locked(struct pinctrl *p,
				       struct pinctrl_state *state)
{
	struct pinctrl_setting *setting, *setting2;
	int ret;

	if (p->state == state)
		return 0;

	if (p->state) {
		/*
                                                                
                                                                 
                                                                 
                                                                 
                                                                  
                                                               
                         
   */
		list_for_each_entry(setting, &p->state->settings, node) {
			bool found = false;
			if (setting->type != PIN_MAP_TYPE_MUX_GROUP)
				continue;
			list_for_each_entry(setting2, &state->settings, node) {
				if (setting2->type != PIN_MAP_TYPE_MUX_GROUP)
					continue;
				if (setting2->data.mux.group ==
						setting->data.mux.group) {
					found = true;
					break;
				}
			}
			if (!found)
				pinmux_disable_setting(setting);
		}
	}

	p->state = state;

	/*                                          */
	list_for_each_entry(setting, &state->settings, node) {
		switch (setting->type) {
		case PIN_MAP_TYPE_MUX_GROUP:
			ret = pinmux_enable_setting(setting);
			break;
		case PIN_MAP_TYPE_CONFIGS_PIN:
		case PIN_MAP_TYPE_CONFIGS_GROUP:
			ret = pinconf_apply_setting(setting);
			break;
		default:
			ret = -EINVAL;
			break;
		}
		if (ret < 0) {
			/*                                          */
			return ret;
		}
	}

	return 0;
}

/* 
                                                                   
                                                                   
                                                      
 */
int pinctrl_select_state(struct pinctrl *p, struct pinctrl_state *state)
{
	int ret;

	mutex_lock(&pinctrl_mutex);
	ret = pinctrl_select_state_locked(p, state);
	mutex_unlock(&pinctrl_mutex);

	return ret;
}
EXPORT_SYMBOL_GPL(pinctrl_select_state);

/* 
                                                                          
                                                                            
                                                                 
                                                                
                                                     
 */
int pinctrl_register_mappings(struct pinctrl_map const *maps,
			      unsigned num_maps)
{
	int i, ret;
	struct pinctrl_maps *maps_node;

	pr_debug("add %d pinmux maps\n", num_maps);

	/*                                    */
	for (i = 0; i < num_maps; i++) {
		if (!maps[i].dev_name) {
			pr_err("failed to register map %s (%d): no device given\n",
			       maps[i].name, i);
			return -EINVAL;
		}

		if (!maps[i].name) {
			pr_err("failed to register map %d: no map name given\n",
			       i);
			return -EINVAL;
		}

		if (maps[i].type != PIN_MAP_TYPE_DUMMY_STATE &&
				!maps[i].ctrl_dev_name) {
			pr_err("failed to register map %s (%d): no pin control device given\n",
			       maps[i].name, i);
			return -EINVAL;
		}

		switch (maps[i].type) {
		case PIN_MAP_TYPE_DUMMY_STATE:
			break;
		case PIN_MAP_TYPE_MUX_GROUP:
			ret = pinmux_validate_map(&maps[i], i);
			if (ret < 0)
				return 0;
			break;
		case PIN_MAP_TYPE_CONFIGS_PIN:
		case PIN_MAP_TYPE_CONFIGS_GROUP:
			ret = pinconf_validate_map(&maps[i], i);
			if (ret < 0)
				return 0;
			break;
		default:
			pr_err("failed to register map %s (%d): invalid type given\n",
			       maps[i].name, i);
			return -EINVAL;
		}
	}

	maps_node = kzalloc(sizeof(*maps_node), GFP_KERNEL);
	if (!maps_node) {
		pr_err("failed to alloc struct pinctrl_maps\n");
		return -ENOMEM;
	}

	maps_node->num_maps = num_maps;
	maps_node->maps = kmemdup(maps, sizeof(*maps) * num_maps, GFP_KERNEL);
	if (!maps_node->maps) {
		pr_err("failed to duplicate mapping table\n");
		kfree(maps_node);
		return -ENOMEM;
	}

	mutex_lock(&pinctrl_mutex);
	list_add_tail(&maps_node->node, &pinctrl_maps);
	mutex_unlock(&pinctrl_mutex);

	return 0;
}

#ifdef CONFIG_DEBUG_FS

static int pinctrl_pins_show(struct seq_file *s, void *what)
{
	struct pinctrl_dev *pctldev = s->private;
	const struct pinctrl_ops *ops = pctldev->desc->pctlops;
	unsigned i, pin;

	seq_printf(s, "registered pins: %d\n", pctldev->desc->npins);

	mutex_lock(&pinctrl_mutex);

	/*                                                                   */
	for (i = 0; i < pctldev->desc->npins; i++) {
		struct pin_desc *desc;

		pin = pctldev->desc->pins[i].number;
		desc = pin_desc_get(pctldev, pin);
		/*                         */
		if (desc == NULL)
			continue;

		seq_printf(s, "pin %d (%s) ", pin,
			   desc->name ? desc->name : "unnamed");

		/*                              */
		if (ops->pin_dbg_show)
			ops->pin_dbg_show(pctldev, s, pin);

		seq_puts(s, "\n");
	}

	mutex_unlock(&pinctrl_mutex);

	return 0;
}

static int pinctrl_groups_show(struct seq_file *s, void *what)
{
	struct pinctrl_dev *pctldev = s->private;
	const struct pinctrl_ops *ops = pctldev->desc->pctlops;
	unsigned selector = 0;

	mutex_lock(&pinctrl_mutex);

	seq_puts(s, "registered pin groups:\n");
	while (ops->list_groups(pctldev, selector) >= 0) {
		const unsigned *pins;
		unsigned num_pins;
		const char *gname = ops->get_group_name(pctldev, selector);
		int ret;
		int i;

		ret = ops->get_group_pins(pctldev, selector,
					  &pins, &num_pins);
		if (ret)
			seq_printf(s, "%s [ERROR GETTING PINS]\n",
				   gname);
		else {
			seq_printf(s, "group: %s, pins = [ ", gname);
			for (i = 0; i < num_pins; i++)
				seq_printf(s, "%d ", pins[i]);
			seq_puts(s, "]\n");
		}
		selector++;
	}

	mutex_unlock(&pinctrl_mutex);

	return 0;
}

static int pinctrl_gpioranges_show(struct seq_file *s, void *what)
{
	struct pinctrl_dev *pctldev = s->private;
	struct pinctrl_gpio_range *range = NULL;

	seq_puts(s, "GPIO ranges handled:\n");

	mutex_lock(&pinctrl_mutex);

	/*                      */
	list_for_each_entry(range, &pctldev->gpio_ranges, node) {
		seq_printf(s, "%u: %s GPIOS [%u - %u] PINS [%u - %u]\n",
			   range->id, range->name,
			   range->base, (range->base + range->npins - 1),
			   range->pin_base,
			   (range->pin_base + range->npins - 1));
	}

	mutex_unlock(&pinctrl_mutex);

	return 0;
}

static int pinctrl_devices_show(struct seq_file *s, void *what)
{
	struct pinctrl_dev *pctldev;

	seq_puts(s, "name [pinmux] [pinconf]\n");

	mutex_lock(&pinctrl_mutex);

	list_for_each_entry(pctldev, &pinctrldev_list, node) {
		seq_printf(s, "%s ", pctldev->desc->name);
		if (pctldev->desc->pmxops)
			seq_puts(s, "yes ");
		else
			seq_puts(s, "no ");
		if (pctldev->desc->confops)
			seq_puts(s, "yes");
		else
			seq_puts(s, "no");
		seq_puts(s, "\n");
	}

	mutex_unlock(&pinctrl_mutex);

	return 0;
}

static inline const char *map_type(enum pinctrl_map_type type)
{
	static const char * const names[] = {
		"INVALID",
		"DUMMY_STATE",
		"MUX_GROUP",
		"CONFIGS_PIN",
		"CONFIGS_GROUP",
	};

	if (type >= ARRAY_SIZE(names))
		return "UNKNOWN";

	return names[type];
}

static int pinctrl_maps_show(struct seq_file *s, void *what)
{
	struct pinctrl_maps *maps_node;
	int i;
	struct pinctrl_map const *map;

	seq_puts(s, "Pinctrl maps:\n");

	mutex_lock(&pinctrl_mutex);

	for_each_maps(maps_node, i, map) {
		seq_printf(s, "device %s\nstate %s\ntype %s (%d)\n",
			   map->dev_name, map->name, map_type(map->type),
			   map->type);

		if (map->type != PIN_MAP_TYPE_DUMMY_STATE)
			seq_printf(s, "controlling device %s\n",
				   map->ctrl_dev_name);

		switch (map->type) {
		case PIN_MAP_TYPE_MUX_GROUP:
			pinmux_show_map(s, map);
			break;
		case PIN_MAP_TYPE_CONFIGS_PIN:
		case PIN_MAP_TYPE_CONFIGS_GROUP:
			pinconf_show_map(s, map);
			break;
		default:
			break;
		}

		seq_printf(s, "\n");
	}

	mutex_unlock(&pinctrl_mutex);

	return 0;
}

static int pinctrl_show(struct seq_file *s, void *what)
{
	struct pinctrl *p;
	struct pinctrl_state *state;
	struct pinctrl_setting *setting;

	seq_puts(s, "Requested pin control handlers their pinmux maps:\n");

	mutex_lock(&pinctrl_mutex);

	list_for_each_entry(p, &pinctrl_list, node) {
		seq_printf(s, "device: %s current state: %s\n",
			   dev_name(p->dev),
			   p->state ? p->state->name : "none");

		list_for_each_entry(state, &p->states, node) {
			seq_printf(s, "  state: %s\n", state->name);

			list_for_each_entry(setting, &state->settings, node) {
				struct pinctrl_dev *pctldev = setting->pctldev;

				seq_printf(s, "    type: %s controller %s ",
					   map_type(setting->type),
					   pinctrl_dev_get_name(pctldev));

				switch (setting->type) {
				case PIN_MAP_TYPE_MUX_GROUP:
					pinmux_show_setting(s, setting);
					break;
				case PIN_MAP_TYPE_CONFIGS_PIN:
				case PIN_MAP_TYPE_CONFIGS_GROUP:
					pinconf_show_setting(s, setting);
					break;
				default:
					break;
				}
			}
		}
	}

	mutex_unlock(&pinctrl_mutex);

	return 0;
}

static int pinctrl_pins_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinctrl_pins_show, inode->i_private);
}

static int pinctrl_groups_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinctrl_groups_show, inode->i_private);
}

static int pinctrl_gpioranges_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinctrl_gpioranges_show, inode->i_private);
}

static int pinctrl_devices_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinctrl_devices_show, NULL);
}

static int pinctrl_maps_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinctrl_maps_show, NULL);
}

static int pinctrl_open(struct inode *inode, struct file *file)
{
	return single_open(file, pinctrl_show, NULL);
}

static const struct file_operations pinctrl_pins_ops = {
	.open		= pinctrl_pins_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations pinctrl_groups_ops = {
	.open		= pinctrl_groups_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations pinctrl_gpioranges_ops = {
	.open		= pinctrl_gpioranges_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations pinctrl_devices_ops = {
	.open		= pinctrl_devices_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations pinctrl_maps_ops = {
	.open		= pinctrl_maps_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static const struct file_operations pinctrl_ops = {
	.open		= pinctrl_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct dentry *debugfs_root;

static void pinctrl_init_device_debugfs(struct pinctrl_dev *pctldev)
{
	struct dentry *device_root;

	device_root = debugfs_create_dir(dev_name(pctldev->dev),
					 debugfs_root);
	pctldev->device_root = device_root;

	if (IS_ERR(device_root) || !device_root) {
		pr_warn("failed to create debugfs directory for %s\n",
			dev_name(pctldev->dev));
		return;
	}
	debugfs_create_file("pins", S_IFREG | S_IRUGO,
			    device_root, pctldev, &pinctrl_pins_ops);
	debugfs_create_file("pingroups", S_IFREG | S_IRUGO,
			    device_root, pctldev, &pinctrl_groups_ops);
	debugfs_create_file("gpio-ranges", S_IFREG | S_IRUGO,
			    device_root, pctldev, &pinctrl_gpioranges_ops);
	pinmux_init_device_debugfs(device_root, pctldev);
	pinconf_init_device_debugfs(device_root, pctldev);
}

static void pinctrl_remove_device_debugfs(struct pinctrl_dev *pctldev)
{
	debugfs_remove_recursive(pctldev->device_root);
}

static void pinctrl_init_debugfs(void)
{
	debugfs_root = debugfs_create_dir("pinctrl", NULL);
	if (IS_ERR(debugfs_root) || !debugfs_root) {
		pr_warn("failed to create debugfs directory\n");
		debugfs_root = NULL;
		return;
	}

	debugfs_create_file("pinctrl-devices", S_IFREG | S_IRUGO,
			    debugfs_root, NULL, &pinctrl_devices_ops);
	debugfs_create_file("pinctrl-maps", S_IFREG | S_IRUGO,
			    debugfs_root, NULL, &pinctrl_maps_ops);
	debugfs_create_file("pinctrl-handles", S_IFREG | S_IRUGO,
			    debugfs_root, NULL, &pinctrl_ops);
}

#else /*                 */

static void pinctrl_init_device_debugfs(struct pinctrl_dev *pctldev)
{
}

static void pinctrl_init_debugfs(void)
{
}

static void pinctrl_remove_device_debugfs(struct pinctrl_dev *pctldev)
{
}

#endif

static int pinctrl_check_ops(struct pinctrl_dev *pctldev)
{
	const struct pinctrl_ops *ops = pctldev->desc->pctlops;

	if (!ops ||
	    !ops->list_groups ||
	    !ops->get_group_name ||
	    !ops->get_group_pins)
		return -EINVAL;

	return 0;
}

/* 
                                                        
                                                
                                              
                                                                    
 */
struct pinctrl_dev *pinctrl_register(struct pinctrl_desc *pctldesc,
				    struct device *dev, void *driver_data)
{
	struct pinctrl_dev *pctldev;
	int ret;

	if (pctldesc == NULL)
		return NULL;
	if (pctldesc->name == NULL)
		return NULL;

	pctldev = kzalloc(sizeof(*pctldev), GFP_KERNEL);
	if (pctldev == NULL) {
		dev_err(dev, "failed to alloc struct pinctrl_dev\n");
		return NULL;
	}

	/*                                      */
	pctldev->owner = pctldesc->owner;
	pctldev->desc = pctldesc;
	pctldev->driver_data = driver_data;
	INIT_RADIX_TREE(&pctldev->pin_desc_tree, GFP_KERNEL);
	INIT_LIST_HEAD(&pctldev->gpio_ranges);
	pctldev->dev = dev;

	/*                           */
	ret = pinctrl_check_ops(pctldev);
	if (ret) {
		pr_err("%s pinctrl ops lacks necessary functions\n",
			pctldesc->name);
		goto out_err;
	}

	/*                                                           */
	if (pctldesc->pmxops) {
		ret = pinmux_check_ops(pctldev);
		if (ret) {
			pr_err("%s pinmux ops lacks necessary functions\n",
			       pctldesc->name);
			goto out_err;
		}
	}

	/*                                                           */
	if (pctldesc->confops) {
		ret = pinconf_check_ops(pctldev);
		if (ret) {
			pr_err("%s pin config ops lacks necessary functions\n",
			       pctldesc->name);
			goto out_err;
		}
	}

	/*                       */
	pr_debug("try to register %d pins on %s...\n",
		 pctldesc->npins, pctldesc->name);
	ret = pinctrl_register_pins(pctldev, pctldesc->pins, pctldesc->npins);
	if (ret) {
		pr_err("error during pin registration\n");
		pinctrl_free_pindescs(pctldev, pctldesc->pins,
				      pctldesc->npins);
		goto out_err;
	}

	mutex_lock(&pinctrl_mutex);

	list_add_tail(&pctldev->node, &pinctrldev_list);

	pctldev->p = pinctrl_get_locked(pctldev->dev);
	if (!IS_ERR(pctldev->p)) {
		struct pinctrl_state *s =
			pinctrl_lookup_state_locked(pctldev->p,
						    PINCTRL_STATE_DEFAULT);
		if (!IS_ERR(s))
			pinctrl_select_state_locked(pctldev->p, s);
	}

	mutex_unlock(&pinctrl_mutex);

	pinctrl_init_device_debugfs(pctldev);

	return pctldev;

out_err:
	kfree(pctldev);
	return NULL;
}
EXPORT_SYMBOL_GPL(pinctrl_register);

/* 
                                           
                                         
  
                                                   
 */
void pinctrl_unregister(struct pinctrl_dev *pctldev)
{
	if (pctldev == NULL)
		return;

	pinctrl_remove_device_debugfs(pctldev);

	mutex_lock(&pinctrl_mutex);

	if (!IS_ERR(pctldev->p))
		pinctrl_put_locked(pctldev->p, true);

	/*                                                */
	list_del(&pctldev->node);
	/*                         */
	pinctrl_free_pindescs(pctldev, pctldev->desc->pins,
			      pctldev->desc->npins);
	kfree(pctldev);

	mutex_unlock(&pinctrl_mutex);
}
EXPORT_SYMBOL_GPL(pinctrl_unregister);

static int __init pinctrl_init(void)
{
	pr_info("initialized pinctrl subsystem\n");
	pinctrl_init_debugfs();
	return 0;
}

/*                                                                       */
core_initcall(pinctrl_init);
