//
//  ssc_media-cpp.h
//  test-ios-sip-sofia
//
//  Created by Antonis Tsakiridis on 9/15/14.
//  Copyright (c) 2014 TeleStax. All rights reserved.
//

#ifndef HAVE_SSC_MEDIA_H
#define HAVE_SSC_MEDIA_H

//#include <iostream>
#include <map>
#include <stdbool.h>
#include <sofia-sip/sdp.h>

#include <unistd.h>

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif

#ifdef HAVE_WS2TCPIP_H
#include <ws2tcpip.h>
#endif

using namespace std;
///G_BEGIN_DECLS

/**
 * Default RTP port range start value for RTP.
 */
#define SSC_MEDIA_RTP_PORT_RANGE_START 16384

enum SscMediaState {
    sm_init = 0,     /**< Media setup ongoing */
    sm_local_ready,  /**< Local resources are set up */
    sm_active,       /**< Media send/recv active */
    sm_error,        /**< Error state has been noticed, client has to call
                      ssc_media_deactivate() */
    sm_disabled
};

class SscMedia
{
public:
    SscMedia();
    ~SscMedia();
    void Deallocate();
    
    int ssc_media_activate(SscMedia *sscm);
    int ssc_media_deactivate(SscMedia *sscm);
    int ssc_media_refresh(SscMedia *sscm);
    int ssc_media_static_capabilities(SscMedia *sscm, char **dest);
    int ssc_media_state(SscMedia *sscm);
    bool ssc_media_is_active(SscMedia *sscm);
    bool ssc_media_is_initialized(SscMedia *sscm);
    void ssc_media_set_remote_to_local(SscMedia *self);
    void ssc_media_set_local_to_caps(SscMedia *sscm);
    
    void ssc_media_set_property (string prop_id, string value);
    string ssc_media_get_property (string prop_id);


    /* signals
     * ------- */
    
    /**
     * A signal emitted whenever the SscMedia's internal state is
     * changed.
     *
     * @see ssc_media_state();
     * @see enum SscMediaState
     */
    void (*state_changed)               (SscMedia      *sscm, int state);
    
    /* methods
     * ------- */
    
    /**
     * Returns description of static capabilities
     * of the media subsystem (codecs, media, and
     * network transports the subsystem could support).
     *
     * The result is a malloc()'ed string, stored to 'dest'.
     *
     * @param self self pointers
     * @param dest where to store pointer to the caps SDP
     */
    int (*static_capabilities)     (SscMedia   *sscm, char **dest);
    
    /**
     * Activates the media subsystem. Causes devices (audio, video) to
     * be opened, and reserving network addresses.
     *
     * @return zero on success
     */
    int (*activate)                (SscMedia   *sscm);
    
    /**
     * Refresh media configuration based on local and remote
     * SDP configuration.
     *
     * @return zero on success
     */
    int (*refresh)                 (SscMedia   *sscm);
    
    /**
     * Deactivates the media subsystem.
     *
     * @pre is_activate() != TRUE
     * @post is_activate() == TRUE
     */
    int (*deactivate)              (SscMedia   *sscm);
    
private:
    bool allocated;
    /* scope/protected:
     * ---------------- */
    map<string, string>     properties;
    int          sm_state;
    
    su_home_t    *sm_home;
    
    sdp_parser_t *sm_sdp_local;
    sdp_parser_t *sm_sdp_remote;
    char        *sm_sdp_local_str;    /**< remote SDP, parsed */
    char        *sm_sdp_remote_str;   /**< remote SDP, raw text */
    
    char         *sm_ad_input_type;
    char         *sm_ad_input_device;
    char         *sm_ad_output_type;
    char         *sm_ad_output_device;

    /* Helper Routines for subclasses */
    /* ------------------------------ */
    
    void ssc_media_signal_state_change(SscMedia *sscm, enum SscMediaState state);

    int priv_set_local_sdp(SscMedia *self, const char *str);
    int priv_set_remote_sdp(SscMedia *self, const char *str);

};




#endif
