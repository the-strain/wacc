/*
 * =====================================================================================
 *
 *       Filename:  appreq_handler.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2015/3/9 13:40:25
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wangxx
 *   Organization:  lj
 *
 * =====================================================================================
 * ============================== CHANGE REPORT HISTORY ================================
 * |   VERSION   |   UPDATE DATE   |   UPDATED BY   |   DESCRIPTION OF CHANGE   |*
 
 * =========================== END OF CHANGE REPORT HISTORY ============================
 */
#if !defined(APPREQHANDLER_H_)
#define APPREQHANDLER_H_
#include "base_collection_handler.h"
#include "msg_list.h"
#include "info_mem_mgr.h"
#include "mysqlconn_wrapper.h"

/*
 * =====================================================================================
 *        Class:  AppReqHandler
 *  Description:  
 * =====================================================================================
 */
class AppReqHandler : public BaseCollectionHandler
{
 public:
	AppReqHandler();                             /* constructor */
	~AppReqHandler();                            /* destructor       */
	int handle_recv();
	int process(char *pmsg);
	int deal_ping(char *data);
	int deal_add_user(char *data);
	int deal_del_user(char *data);
	int deal_MO(char *data);
	int deal_mt_ack(char *data);
	int deal_notify_active(char *data);
	unsigned int generate_tid();

	void info_mgr(InfoMemMgr *p);
	void app_req_queue(MsgList *p);
	void client_list(map<int, BaseCollectionHandler*> *list);
	void db(MySQLConnWrapper *p);
	void sync_data();
	char* msisdn() {return msisdn_;}
 protected:
	unsigned long long ntoh64(unsigned long long inval);
	bool StrToBCD(const char *Src,unsigned char *Des,int iDesLen);
 private:
	static const unsigned int kBufferSize = 4000;
	char recv_buf_[kBufferSize];
	unsigned int offset_;
	InfoMemMgr *info_mgr_;
	MsgList *app_req_queue_;
	map<int, BaseCollectionHandler*> *client_list_;
	char msisdn_[32];
	unsigned char bcd_buf_[256];
	char send_buf_[kBufferSize];
	unsigned int send_offset_;
	MySQLConnWrapper* db_;
}; /* -----  end of class AppReqHandler  ----- */

#endif

