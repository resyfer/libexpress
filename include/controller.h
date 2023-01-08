#ifndef __CONTROLLER_LIBEXRESS_H
#define __CONTROLLER_LIBEXRESS_H 1

#include "err.h"
#include "req.h"
#include "res.h"

void next(res_t *res);

typedef void (controller_t) (req_t *req, res_t *res /*, err_t err */);

#endif