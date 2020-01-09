/*
 * Copyright (c) 2013-2015 Intel Corporation.  All rights reserved.
 *
 * This software is available to you under the BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AWV
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <rdma/fi_errno.h>

#include <shared.h>

static struct fi_rma_iov remote;

static int run_test(void)
{
	int ret, i;

	ret = ft_sync();
	if (ret)
		return ret;

	ft_start();
	for (i = 0; i < opts.iterations; i++) {
		ret = ft_rma(opts.rma_op, ep, opts.transfer_size, &remote, ep);
		if (ret)
			return ret;
	}
	ft_stop();

	if (opts.machr)
		show_perf_mr(opts.transfer_size, opts.iterations, &start, &end,
				1, opts.argc, opts.argv);
	else
		show_perf(NULL, opts.transfer_size, opts.iterations,
				&start, &end, 1);

	return 0;
}

static int run(void)
{
	int i, ret = 0;

	ret = ft_init_fabric();
	if (ret)
		return ret;

	ret = ft_exchange_keys(&remote);
	if (ret)
		goto out;

	if (!(opts.options & FT_OPT_SIZE)) {
		for (i = 0; i < TEST_CNT; i++) {
			if (!ft_use_size(i, opts.sizes_enabled))
				continue;
			opts.transfer_size = test_size[i].size;
			init_test(&opts, test_name, sizeof(test_name));
			ret = run_test();
			if (ret)
				goto out;
		}
	} else {
		init_test(&opts, test_name, sizeof(test_name));
		ret = run_test();
		if (ret)
			goto out;
	}

	ft_sync();
	ft_finalize();
out:
	return ret;
}

int main(int argc, char **argv)
{
	int op, ret;

	opts = INIT_OPTS;

	hints = fi_allocinfo();
	if (!hints)
		return EXIT_FAILURE;

	hints->caps = FI_MSG | FI_RMA;
	hints->ep_attr->type = FI_EP_RDM;
	hints->domain_attr->mr_mode = FI_MR_LOCAL | OFI_MR_BASIC_MAP;

	while ((op = getopt(argc, argv, "ho:" CS_OPTS INFO_OPTS)) != -1) {
		switch (op) {
		default:
			ft_parseinfo(op, optarg, hints);
			ft_parsecsopts(op, optarg, &opts);
			ret = ft_parse_rma_opts(op, optarg, hints, &opts);
			if (ret)
				return ret;
			break;
		case '?':
		case 'h':
			ft_csusage(argv[0], "Ping pong client and server using rma.");
			fprintf(stderr, "  -o <op>\trma op type: read|write|writedata (default: write)]\n");
			return EXIT_FAILURE;
		}
	}

	if (optind < argc)
		opts.dst_addr = argv[optind];

	ret = run();

	ft_free_res();
	return ft_exit_code(ret);
}
