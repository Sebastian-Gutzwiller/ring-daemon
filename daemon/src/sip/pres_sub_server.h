/*
 *  Copyright (C) 2004-2013 Savoir-Faire Linux Inc.
 *
 *  Author: Patrick Keroulas  <patrick.keroulas@savoirfairelinux.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *  Additional permission under GNU GPL version 3 section 7:
 *
 *  If you modify this program, or any covered work, by linking or
 *  combining it with the OpenSSL project's OpenSSL library (or a
 *  modified version of that library), containing parts covered by the
 *  terms of the OpenSSL or SSLeay licenses, Savoir-Faire Linux Inc.
 *  grants you additional permission to convey the resulting work.
 *  Corresponding Source for a non-source form of such a combination
 *  shall include the source code for the parts of OpenSSL used as well
 *  as that of the covered work.
 */


#ifndef SERVERPRESENCESUB_H
#define	SERVERPRESENCESUB_H

#include <pj/string.h>
#include <pjsip/sip_types.h>
#include <pjsip-simple/evsub.h>
#include <pjsip-simple/presence.h>
#include <pjsip/sip_module.h>

#include "src/noncopyable.h"

extern pj_bool_t pres_on_rx_subscribe_request(pjsip_rx_data *rdata);

static pjsip_module mod_presence_server = {
    NULL, NULL, /* prev, next.		*/
    pj_str("mod-presence-server"), /* Name.		*/
    -1, /* Id			*/
    PJSIP_MOD_PRIORITY_DIALOG_USAGE,
    NULL, /* load()		*/
    NULL, /* start()		*/
    NULL, /* stop()		*/
    NULL, /* unload()		*/
    &pres_on_rx_subscribe_request, /* on_rx_request()	*/
    NULL, /* on_rx_response()	*/
    NULL, /* on_tx_request.	*/
    NULL, /* on_tx_response()	*/
    NULL, /* on_tsx_state()	*/

};


class SIPpresence;

class PresSubServer {

public:

    PresSubServer(SIPPresence * pres, pjsip_evsub *evsub, char *r,pjsip_dialog *d);
    ~PresSubServer();
    /* TODO: add '< >' to URI for consistance*/
    char *remote;    /**< Remote URI.			    */
    /*
     * Acces to the evsub expire variable.
     * It was recieved in the SUBSCRIBE request.
     */
    void setExpires(int ms);
    int getExpires();
    /*
     * Match method
     * s is the URI (remote)
     */
    bool matches(char *s);
    /*
     * Allow the subscriber for being notified.
     */
    void approve(const bool& flag);
    /*
     * Notify subscriber with the pres_status_date of the account
     */
    void notify();

    friend void pres_evsub_on_srv_state( pjsip_evsub *sub, pjsip_event *event);
    friend pj_bool_t pres_on_rx_subscribe_request(pjsip_rx_data *rdata);

private:

    NON_COPYABLE(PresSubServer);
    SIPPresence     *pres_;
    pjsip_evsub	    *sub;	    /**< The evsub.			    */
    pjsip_dialog    *dlg;	    /**< Dialog.			    */
    int		    expires;	    /**< "expires" value in the request.    */
    bool            approved;        /**< The client approved this subscription*/
};


#endif	/* SERVERPRESENCESUB_H */