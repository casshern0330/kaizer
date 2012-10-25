#ifndef _UDP_RTT_HEAD
#define _UDP_RTT_HEAD

#include "unp.h"

struct rtt_info{
	float	rtt_rtt;	//moast recent rtt measure time
	float	rtt_srtt;	//smoothed rtt estimated
	float	rtt_rttval;
	float	rtt_rto;	//current RTO to use,seconds
	int	rtt_nrexmt;	//#times,retransmitted times
	uint32_t	rtt_base;	//#sec since 1/1/1970
};

#define RTT_RXTMIN	2	//min retransmitted timeout,#second
#define RTT_RXTMAX	60	//max retransmitted timeout,#second
#define RTT_MAXNREXMT	3	//max #times retransmitted

/*function description*/
void rtt_debug(struct rtt_info *);
void rtt_init(struct rtt_info *);
void rtt_newpack(struct rtt_info *);
void rtt_start(struct rtt_info *);
void rtt_stop(struct rtt_info *,uint32_t);
void rtt_timeout(struct rtt_info *);
uint32_t rtt_ts(struct rtt_info *);

extern int	rtt_d_flags;	//can be set nonzero for add info
#endif
