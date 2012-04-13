/**
 * Seccomp Library test program
 *
 * Copyright IBM Corp. 2012
 * Author: Ashley Lai <adlai@us.ibm.com>
 */

/*
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License as
 * published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <http://www.gnu.org/licenses>.
 */

#include <unistd.h>

#include <seccomp.h>

#include "util.h"

int main(int argc, char *argv[])
{
	int rc;
	struct util_options opts;

	rc = util_getopt(argc, argv, &opts);
	if (rc < 0)
		return rc;

	rc = seccomp_init(SCMP_ACT_KILL);
	if (rc != 0)
		return rc;

	/* The next three seccomp_rule_add_exact() calls for read must
	 * go together in this order to catch an infinite loop. */

	rc = seccomp_rule_add_exact(SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
				    SCMP_A0(SCMP_CMP_EQ, STDOUT_FILENO));
	if (rc != 0)
		return rc;

	rc = seccomp_rule_add_exact(SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
				    SCMP_A1(SCMP_CMP_EQ, 0x0));
	if (rc != 0)
		return rc;

	rc = seccomp_rule_add_exact(SCMP_ACT_ALLOW, SCMP_SYS(read), 1,
				    SCMP_A0(SCMP_CMP_EQ, STDIN_FILENO));
	if (rc != 0)
		return rc;

	rc = util_filter_output(&opts);
	if (rc)
		return rc;

	seccomp_release();
	return rc;
}
