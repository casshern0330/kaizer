#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<atmi.h>
#include "general32.flds.h"
#include "ferror32.h"

#if defined(_STDC_) || defined(_cplusplus)
main(int argc,char *argv[])
#else
main(argc,argv)
int argc;
char *argv[];
#endif
{
    FBFR32 *sendbuf,*rcvbuf;
    FLDLEN32 sendlen,rcvlen;
    int i,j,occs,sendnum,rcvnum,ret;
    char servicename[32],buffer[1024];
    long reqlen;
    
    if(argc < 4)
    {
        fprintf(stderr,"Usage: cl32 servicename ,SEND_PARAM_NUM RECV_PARAM_NUM, arg1,arg2,...\n");
        exit(1);
    }
    
    memset(servicename,0,sizeof(servicename));
    strcpy(servicename,argv[1]);
    
    sendnum = atoi(argv[2]);
    recvnum = atoi(argv[3]);
    
    if(tpinit((TPINIT *) NULL) == -1){
        fprintf(stderr,"Tpinit failed [%s]\n",tpstrerror(tperrno));
        exit(1);
    }
    
    sendlen = (FLDLEN32)((sendnum)*100);
    //初始化输出数据空间
    if((sendbuf = (FBFR32 *)tpalloc(FMLTYPE32,NULL,sendlen)) == NULL){
        fprintf(stderr,"error allocating send buffer");
        tpterm();   //结束操作,与tpinit()匹配
        exit(1);
    }
    
    Fchg32(sendbuf,SEND_PARAM_NUM32,0,argv[2],(FLDLEN32)0);
    Fchg32(sendbuf,RECP_PARAM_NUM32,0,argv[3],(FLDLEN32)0);
    
    for(i=0 ; i<sendnum; i++){
        printf("SEND GPRAM32_[%d]=[%s]\n",i,argv[4+i]);
        //sendbuf,设置空间数据
        Fchg32(sendbuf,GPARM32_0 + i,0,argv[4+i],(FLDLEN32)0);
    }
    
    rcvlen = (FLDLEN32)((rcvnum)*100);
    //初始化接收参数空间
    if((rcvbuf = (FBFR32 *)tpalloc(FMLTYPE2,NULL,rcvlen)) == NULL){
        fprintf(stderr,"error allocating rcv buffer");
        tpfree((char *)sendbuf);
        tpterm();
        exit(1);
    }
    
    reqlen = Fsizeof32(rcvbuf);
    ret = tpcall(servicename,(char *)sendbuf,0,(char **)&rcvbuf,&reqlen,(long)0);
    
    //tpreturn(TPSUCCESS,0,(char *)transOut,0L,0);
    printf("ret=[%d]\n",ret);
    if(ret == -1)
    {
        fprintf(stderr,"can't send request to service TOUPPER.\n");
        fprintf(stderr,"Tperrno = %d\n",tperrno);
        tpfree((char *)sendbuf);
        tpfree((char *)rcvbuf);
        tpterm();
        exit(1);
    }
    
    printf("++++ tpcall succ ++++\n");
    
    //获取错误代码
    //Fchg32(transOut,SVC_ERR_NO32,0,retCode,(FLDLEN32)0);
    Fget32(rcvbuf,SVC_ERR_NO32,0,buffer,NULL);
    printf("++ SVC_ERR_NO32 = [%s] \n",buffer);
    //获取错误信息
    //Fchg32(transOut,SVC_ERR_MSG32,0,retMsg,(FLDLEN32)0);
    Fget32(rcvbuf,SVC_ERR_MSG32,0,buffer,NULL);
    printf("++ SVC_ERR_MSG32 = [%s] \n",buffer);
    memset(buffer,0,sizeof(buffer));
    
    
    for(i=0;i<recvnum;i++){
        occs = 0;
        //occs>1,处理结果集一个数组
        occs = Foccs32(rcvbuf,GPARM32_0 +i);
        if(occs > 1)
        {
            for(j=0;j<occs;j++)
            {
                Fget32(rcvbuf,GPARM32_0 + i,j,buffer,NULL);
                printf("+RECV GPARM32_[%d][%d]=[%s]\n",i,j,buffer);
                memset(buffer,0,sizeof(buffer));
            }
        } else
        {
            Fget32(rcvbuf,GPARM32_0 + i,0,buffer,NULL);
            printf("+RECV GPARAM32_[%d]=[%s]\n",i,buffer);
            memset(buffer,0,sizeof(buffer));
        }
    }
    
    tpfree((char *)sendbuf);
    tpfree((char *)rcvbuf);
    tpterm();
    return 0;
}