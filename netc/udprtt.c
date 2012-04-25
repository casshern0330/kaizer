#include "unprrt.h"

#define RTT_RTOCACL(rtt)	((rtt)->rtt_srtt + (4.00 * (rtt)->rtt_rttval))

float minmaxrtt(float ms)
	float rtt;	
	rtt = ms;
	if(rtt < RTT_RXTMIN)
		rtt = RTT_RXTMIN;
	else if(rtt > RTT_RXTMAX)
		rtt = RTT_RXTMAX;
	return rtt;
}

void rtt_init(struct rtt_info *rtt)
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	rtt->rtt_base = tv.tv_sec;
	rtt->rtt_rtt = 0;
	rtt->rtt_srtt = 0;
	rtt->rtt_rttval = 0.75;
	rtt->rtt_rto = minmaxrtt(RTT_RTOCALC(rtt));
}

//caculte time between now and rtt_init
uint32_t rtt_ts(struct rtt_info *rtt){
	uint32_t ts;
	struct timeval tv;
	gettimeofday(&tv,NULL);
	ts = ((tv->tv_sec - rtt->rtt_base)*1000) + (tv->tv_usec/1000)
}

void rtt_newpack(struct rtt_info *rtt)
{
	rtt->rtt_nrexmt = 0;
}

int rtt_start(struct rtt_info *rtt)
{
	return (int)(rtt->rtt_rto + 0.5);
}

void rtt_stop(struct rtt_info *rtt,uint32_t ms)
{
	double delta;
	rtt->rtt_rtt = ms/1000;
	delta = rtt->rtt_rtt - rtt->rtt_srtt;
	rtt->rtt_srtt = delta/8;
	delta = delta < 0 ? -delta : delta;
	rtt->rtt_rttval += (delta - rtt->rtt_srtt)/4;
	rtt->rtt_rto = minmaxrtt(RTT_RTOCALC(rtt));
}

int rtt_timeout(struct rtt_info *rtt)
{
	rtt->rtt_rto *= 2;	/*calculate next rto timeval*/
	if(++rtt->rtt_nrexmt > RTT_MAXNREXMT)
		return (-1);
	return (0);
}

