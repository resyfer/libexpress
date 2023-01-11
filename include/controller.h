/**
 * @file controller.h
 * @brief This contains all the controller related material.
 *
 * @author Saurav Pal (resyfer)
 * @bug No known bugs
 */
#ifndef __CONTROLLER_LIBEXRESS_H
#define __CONTROLLER_LIBEXRESS_H 1

#include "req.h"
#include "res.h"

typedef void (controller_t) (req_t *req, res_t *res);
typedef controller_t middleware_t;

#endif