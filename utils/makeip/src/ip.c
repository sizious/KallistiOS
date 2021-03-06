/* IP creator (makeip)
 *
 * Copyright (C) 2000, 2001, 2002, 2019, 2020 The KOS Team and contributors.
 * All rights reserved.
 *
 * This code was contributed to KallistiOS (KOS) by Andress Antonio Barajas
 * (BBHoodsta). It was originally made by Marcus Comstedt (zeldin). Some
 * portions of code were made by Andrew Kieschnick (ADK/Napalm). Heavily
 * updated by SiZiOUS. Bootstrap replacement (IP.TMPL) was made by Jacob
 * Alberty (LiENUS).
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "ip.h"

void
ip_read(char *ip, char *fn_iptmpl)
{
  FILE *fh = fopen(fn_iptmpl, "rb");

  if(fh == NULL) {
    halt("can't open bootstrap template: \"%s\"\n", fn_iptmpl);
  }

  int success = 1;
  if(fread(ip, 1, INITIAL_PROGRAM_SIZE, fh) != INITIAL_PROGRAM_SIZE) {
    success = 0;
  }

  fclose(fh);

  if (!success) {
    halt("bootstrap template: read error or wrong input file size\n");
  } else {
    log_notice("successfully replaced default bootstrap template with \"%s\"\n", fn_iptmpl);
  }
}

void
ip_write(char *ip, char *fn_ipout, char *fn_imgin, char *fn_imgout)
{
  FILE *fh;

  update_crc(ip);

  if(fn_imgin != NULL) {
    mr_inject(ip, fn_imgin, fn_imgout);
  }

  fh = fopen(fn_ipout, "wb");
  if(fh == NULL) {
    halt("can't open \"%s\" in write mode\n", fn_ipout);
  }

  int result = 1;
  if(fwrite(ip, 1, INITIAL_PROGRAM_SIZE, fh) != INITIAL_PROGRAM_SIZE) {
    result = 0;
  }

  fclose(fh);

  if (!result) {
    halt("output write error: %s\n", strerror(errno));
  }
}
