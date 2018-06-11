#ifndef STONECOIN_UPDATER_H
#define STONECOIN_UPDATER_H

#include <stdio.h>
#include "chainparamsbase.h"
#include "clientversion.h"
#include "rpcclient.h"
#include "rpcprotocol.h"
#include "util.h"
#include "utilstrencodings.h"
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iostream>

#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>

#include <univalue.h>
#include "../versionbits.h"

/* Internal use only: Functions that might be missing from <sys/queue.h> */
#ifndef TAILQ_FIRST
#define TAILQ_FIRST(head) ((head)->tqh_first)
#endif
#ifndef TAILQ_END
#define TAILQ_END(head) NULL
#endif
#ifndef TAILQ_NEXT
#define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)
#endif

#ifndef TAILQ_FOREACH
#define TAILQ_FOREACH(var, head, field) \
    for ((var) = TAILQ_FIRST(head);     \
         (var) != TAILQ_END(head);      \
         (var) = TAILQ_NEXT(var, field))
#endif

void downloadUpdate(Consensus::Params* params);


#endif