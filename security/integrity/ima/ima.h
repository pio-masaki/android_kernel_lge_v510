/*
 * Copyright (C) 2005,2006,2007,2008 IBM Corporation
 *
 * Authors:
 * Reiner Sailer <sailer@watson.ibm.com>
 * Mimi Zohar <zohar@us.ibm.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
 *
 * File: ima.h
 *	internal Integrity Measurement Architecture (IMA) definitions
 */

#ifndef __LINUX_IMA_H
#define __LINUX_IMA_H

#include <linux/types.h>
#include <linux/crypto.h>
#include <linux/security.h>
#include <linux/hash.h>
#include <linux/tpm.h>
#include <linux/audit.h>

#include "../integrity.h"

enum ima_show_type { IMA_SHOW_BINARY, IMA_SHOW_ASCII };
enum tpm_pcrs { TPM_PCR0 = 0, TPM_PCR8 = 8 };

/*                                       */
#define IMA_DIGEST_SIZE		SHA1_DIGEST_SIZE
#define IMA_EVENT_NAME_LEN_MAX	255

#define IMA_HASH_BITS 9
#define IMA_MEASURE_HTABLE_SIZE (1 << IMA_HASH_BITS)

/*                           */
extern int ima_initialized;
extern int ima_used_chip;
extern char *ima_hash;

/*                               */
struct ima_template_data {
	u8 digest[IMA_DIGEST_SIZE];	/*                           */
	char file_name[IMA_EVENT_NAME_LEN_MAX + 1];	/*           */
};

struct ima_template_entry {
	u8 digest[IMA_DIGEST_SIZE];	/*                              */
	const char *template_name;
	int template_len;
	struct ima_template_data template;
};

struct ima_queue_entry {
	struct hlist_node hnext;	/*                              */
	struct list_head later;		/*                                */
	struct ima_template_entry *entry;
};
extern struct list_head ima_measurements;	/*                          */

/*              */
void integrity_audit_msg(int audit_msgno, struct inode *inode,
			 const unsigned char *fname, const char *op,
			 const char *cause, int result, int info);

/*                                   */
int ima_init(void);
void ima_cleanup(void);
int ima_fs_init(void);
void ima_fs_cleanup(void);
int ima_inode_alloc(struct inode *inode);
int ima_add_template_entry(struct ima_template_entry *entry, int violation,
			   const char *op, struct inode *inode);
int ima_calc_hash(struct file *file, char *digest);
int ima_calc_template_hash(int template_len, void *template, char *digest);
int ima_calc_boot_aggregate(char *digest);
void ima_add_violation(struct inode *inode, const unsigned char *filename,
		       const char *op, const char *cause);

/*
                                        
 */
extern spinlock_t ima_queue_lock;

struct ima_h_table {
	atomic_long_t len;	/*                                           */
	atomic_long_t violations;
	struct hlist_head queue[IMA_MEASURE_HTABLE_SIZE];
};
extern struct ima_h_table ima_htable;

static inline unsigned long ima_hash_key(u8 *digest)
{
	return hash_long(*digest, IMA_HASH_BITS);
}

/*                              */
int ima_must_measure(struct inode *inode, int mask, int function);
int ima_collect_measurement(struct integrity_iint_cache *iint,
			    struct file *file);
void ima_store_measurement(struct integrity_iint_cache *iint, struct file *file,
			   const unsigned char *filename);
int ima_store_template(struct ima_template_entry *entry, int violation,
		       struct inode *inode);
void ima_template_show(struct seq_file *m, void *e, enum ima_show_type show);

/*                                            
                                           
 */
struct integrity_iint_cache *integrity_iint_insert(struct inode *inode);
struct integrity_iint_cache *integrity_iint_find(struct inode *inode);

/*                              */
enum ima_hooks { FILE_CHECK = 1, FILE_MMAP, BPRM_CHECK };

int ima_match_policy(struct inode *inode, enum ima_hooks func, int mask);
void ima_init_policy(void);
void ima_update_policy(void);
ssize_t ima_parse_add_rule(char *);
void ima_delete_rules(void);

/*                                      */
#ifdef CONFIG_IMA_LSM_RULES

#define security_filter_rule_init security_audit_rule_init
#define security_filter_rule_match security_audit_rule_match

#else

static inline int security_filter_rule_init(u32 field, u32 op, char *rulestr,
					    void **lsmrule)
{
	return -EINVAL;
}

static inline int security_filter_rule_match(u32 secid, u32 field, u32 op,
					     void *lsmrule,
					     struct audit_context *actx)
{
	return -EINVAL;
}
#endif /*                      */
#endif
