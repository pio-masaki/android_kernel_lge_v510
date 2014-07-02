/*
 * Squashfs - a compressed read only filesystem for Linux
 *
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008
 * Phillip Lougher <phillip@squashfs.org.uk>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * namei.c
 */

/*
                                                                  
  
                                                                              
                                                                             
                                                                 
                                        
  
                                                                          
                                                                 
                                                                        
                                                                       
                                                                       
                                                                              
                                                                               
                                                                   
                                                                      
  
                                                                        
                                                                            
                                                                   
                                                                         
                                                                             
                                                                              
                                                                    
                                                                              
                                                                           
                                                                              
                                                  
 */

#include <linux/fs.h>
#include <linux/vfs.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/dcache.h>
#include <linux/xattr.h>

#include "squashfs_fs.h"
#include "squashfs_fs_sb.h"
#include "squashfs_fs_i.h"
#include "squashfs.h"
#include "xattr.h"

/*
                                                                             
                                                                
  
                                                                         
                                                                     
           
 */
static int get_dir_index_using_name(struct super_block *sb,
			u64 *next_block, int *next_offset, u64 index_start,
			int index_offset, int i_count, const char *name,
			int len)
{
	struct squashfs_sb_info *msblk = sb->s_fs_info;
	int i, size, length = 0, err;
	struct squashfs_dir_index *index;
	char *str;

	TRACE("Entered get_dir_index_using_name, i_count %d\n", i_count);

	index = kmalloc(sizeof(*index) + SQUASHFS_NAME_LEN * 2 + 2, GFP_KERNEL);
	if (index == NULL) {
		ERROR("Failed to allocate squashfs_dir_index\n");
		goto out;
	}

	str = &index->name[SQUASHFS_NAME_LEN + 1];
	strncpy(str, name, len);
	str[len] = '\0';

	for (i = 0; i < i_count; i++) {
		err = squashfs_read_metadata(sb, index, &index_start,
					&index_offset, sizeof(*index));
		if (err < 0)
			break;


		size = le32_to_cpu(index->size) + 1;

		err = squashfs_read_metadata(sb, index->name, &index_start,
					&index_offset, size);
		if (err < 0)
			break;

		index->name[size] = '\0';

		if (strcmp(index->name, str) > 0)
			break;

		length = le32_to_cpu(index->index);
		*next_block = le32_to_cpu(index->start_block) +
					msblk->directory_table;
	}

	*next_offset = (length + *next_offset) % SQUASHFS_METADATA_SIZE;
	kfree(index);

out:
	/*
                                                                    
                                                                      
                                                                       
              
  */
	return length + 3;
}


static struct dentry *squashfs_lookup(struct inode *dir, struct dentry *dentry,
				 struct nameidata *nd)
{
	const unsigned char *name = dentry->d_name.name;
	int len = dentry->d_name.len;
	struct inode *inode = NULL;
	struct squashfs_sb_info *msblk = dir->i_sb->s_fs_info;
	struct squashfs_dir_header dirh;
	struct squashfs_dir_entry *dire;
	u64 block = squashfs_i(dir)->start + msblk->directory_table;
	int offset = squashfs_i(dir)->offset;
	int err, length, dir_count, size;

	TRACE("Entered squashfs_lookup [%llx:%x]\n", block, offset);

	dire = kmalloc(sizeof(*dire) + SQUASHFS_NAME_LEN + 1, GFP_KERNEL);
	if (dire == NULL) {
		ERROR("Failed to allocate squashfs_dir_entry\n");
		return ERR_PTR(-ENOMEM);
	}

	if (len > SQUASHFS_NAME_LEN) {
		err = -ENAMETOOLONG;
		goto failed;
	}

	length = get_dir_index_using_name(dir->i_sb, &block, &offset,
				squashfs_i(dir)->dir_idx_start,
				squashfs_i(dir)->dir_idx_offset,
				squashfs_i(dir)->dir_idx_cnt, name, len);

	while (length < i_size_read(dir)) {
		/*
                           
   */
		err = squashfs_read_metadata(dir->i_sb, &dirh, &block,
				&offset, sizeof(dirh));
		if (err < 0)
			goto read_failure;

		length += sizeof(dirh);

		dir_count = le32_to_cpu(dirh.count) + 1;

		if (dir_count > SQUASHFS_DIR_COUNT)
			goto data_error;

		while (dir_count--) {
			/*
                           
    */
			err = squashfs_read_metadata(dir->i_sb, dire, &block,
					&offset, sizeof(*dire));
			if (err < 0)
				goto read_failure;

			size = le16_to_cpu(dire->size) + 1;

			/*                                                    */
			if (size > SQUASHFS_NAME_LEN)
				goto data_error;

			err = squashfs_read_metadata(dir->i_sb, dire->name,
					&block, &offset, size);
			if (err < 0)
				goto read_failure;

			length += sizeof(*dire) + size;

			if (name[0] < dire->name[0])
				goto exit_lookup;

			if (len == size && !strncmp(name, dire->name, len)) {
				unsigned int blk, off, ino_num;
				long long ino;
				blk = le32_to_cpu(dirh.start_block);
				off = le16_to_cpu(dire->offset);
				ino_num = le32_to_cpu(dirh.inode_number) +
					(short) le16_to_cpu(dire->inode_number);
				ino = SQUASHFS_MKINODE(blk, off);

				TRACE("calling squashfs_iget for directory "
					"entry %s, inode  %x:%x, %d\n", name,
					blk, off, ino_num);

				inode = squashfs_iget(dir->i_sb, ino, ino_num);
				goto exit_lookup;
			}
		}
	}

exit_lookup:
	kfree(dire);
	return d_splice_alias(inode, dentry);

data_error:
	err = -EIO;

read_failure:
	ERROR("Unable to read directory block [%llx:%x]\n",
		squashfs_i(dir)->start + msblk->directory_table,
		squashfs_i(dir)->offset);
failed:
	kfree(dire);
	return ERR_PTR(err);
}


const struct inode_operations squashfs_dir_inode_ops = {
	.lookup = squashfs_lookup,
	.getxattr = generic_getxattr,
	.listxattr = squashfs_listxattr
};
