/*
 * =========================================================
 *
 *       Filename:  base_collection_handler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013.6.14
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wangxx
 *   Organization:  lj
 *
 * ==========================================================
 * ================== CHANGE REPORT HISTORY ==================
 *|VERSION|UPDATE DATE|UPDATED BY|DESCRIPTION OF CHANGE|*

 * ================== END OF CHANGE REPORT HISTORY ===========
 */
#include "base_collection_handler.h"
#include "common_logger.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>


BaseCollectionHandler::BaseCollectionHandler() {

}

BaseCollectionHandler::~BaseCollectionHandler() {

}

int BaseCollectionHandler::recvn(char *buf, size_t recv_size) {
	int readed_size = 0;
	int need_read_size = recv_size;
	int ret;
	while (need_read_size>0) {
		ret = recv(sockfd_, buf+readed_size, need_read_size, 0);
		if (ret<0) {
			int bugno=errno;//errno 记录最后一次系统错误码，所以最好先保存下来added by wangxx 20160922
			if (bugno == EINTR) {
			} else if (bugno == EAGAIN) {
				break;
			} else {
				CommonLogger::instance().log_error("[%s %d] BaseCollectionHandler::recvn  errno=%d  return -1", __FILE__,__LINE__,bugno);
				return -1;
			}
		} else if (ret == 0) {
			CommonLogger::instance().log_error("[%s %d] BaseCollectionHandler::recvn  ret == 0, return -1", __FILE__,__LINE__);
			return -1;
		}

		need_read_size -= ret;
		readed_size += ret;
	}
	return readed_size;
}


//add by chail on 20131124
int BaseCollectionHandler::sendn(const char *buf, size_t send_size)
{
    unsigned int need_send_length = send_size;
    int sended_length=0;
    CommonLogger::instance().log_error("BaseCollectionHandler::sendn: sockfd_ is %u",sockfd_);
    while (need_send_length>0) {
        int ret = send(sockfd_,buf+sended_length,need_send_length,0);
        if (ret<0) {
            if(errno == EINTR){
            } else if(errno == EAGAIN) {
                break;
            } else {
                return -1;
            }
        }else if(ret == 0){
            return -1;
        }

        sended_length+=ret;
        need_send_length-=ret;
    }
    return sended_length;
}

int BaseCollectionHandler::logout(const char *buf, size_t send_size)
{
	sendn(buf, send_size);
	return 0;
}