/*****************************************************************************\
 *  proc_msg.h - process incoming message functions
 *****************************************************************************
 *  Copyright (C) 2002 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov> and Kevin Tew <tew1@llnl.gov>
 *  CODE-OCEC-09-009. All rights reserved.
 *
 *  This file is part of Slurm, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  Slurm is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#ifndef _HAVE_PROC_REQ_H
#define _HAVE_PROC_REQ_H

#include <sys/time.h>

#include "src/common/slurm_protocol_api.h"

#include "src/slurmctld/locks.h"

typedef struct {
	uint16_t msg_type;
	void (*func)(slurm_msg_t *msg);
	void (*post_func)();
	slurmctld_lock_t locks;

	/* Queue structual elements */
	char *msg_name; /* automatically derived from msg_type */

	bool queue_enabled;
	bool shutdown;

	pthread_t thread;
	pthread_cond_t cond;
	pthread_mutex_t mutex;

	List work;
} slurmctld_rpc_t;

extern slurmctld_rpc_t slurmctld_rpcs[];

/*
 * slurmctld_req  - Process an individual RPC request
 * IN/OUT msg - the request message, data associated with the message is freed
 */
void slurmctld_req(slurm_msg_t *msg);

/*
 * Update slurmctld stats structure with time spent processing an rpc.
 */
extern void record_rpc_stats(slurm_msg_t *msg, long delta);

/*
 * Initialize a response slurm_msg_t to an inbound msg,
 * first by calling slurm_msg_t_init(), then by copying
 * fields needed to communicate with the remote correctly.
 */
extern void response_init(slurm_msg_t *resp, slurm_msg_t *msg);

/* Copy an array of type char **, xmalloc() the array and xstrdup() the
 * strings in the array */
extern char **xduparray(uint32_t size, char ** array);

/*
 * build_alloc_msg - Fill in resource_allocation_response_msg_t off job_record.
 * job_ptr IN - job_record to copy members off.
 * error_code IN - error code used for the response.
 * job_submit_user_msg IN - user message from job submit plugin.
 * RET resource_allocation_response_msg_t filled in.
 */
extern resource_allocation_response_msg_t *build_alloc_msg(
	job_record_t *job_ptr, int error_code, char *job_submit_user_msg);

#ifdef __METASTACK_OPT_CACHE_QUERY	

/*
 * slurmctld_query_req  - Process an individual RPC request
 * IN/OUT msg - the request message, data associated with the message is freed
 */
void slurmctld_query_req(slurm_msg_t *msg);
#endif

#endif /* !_HAVE_PROC_REQ_H */
