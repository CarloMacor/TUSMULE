#ifndef PROTOCOL_DEFS_H
#define PROTOCOL_DEFS_H

//==========================================================
// Commands
//==========================================================

#define CMD_HELLO          "HELLO"
#define CMD_PING           "PING"
#define CMD_INFO           "INFO"
#define CMD_COUNT          "COUNT"
#define CMD_GET            "GET "
#define CMD_BEGIN          "BEGIN"
#define CMD_NEXT           "NEXT"
#define CMD_NEXTBIN        "NEXTBIN"
#define CMD_END            "END"
#define CMD_BYE            "BYE"
#define CMD_TIME           "TIME="
#define CMD_NEXTFLIGHT     "NEXTFLIGHT="

//==========================================================
// Replies
//==========================================================

#define RSP_OK             "OK"
#define RSP_ERR            "ERR"
#define RSP_END            "END"
#define RSP_PONG           "PONG"
#define RSP_BYE            "BYE"
#define RSP_NOSESSION      "NOSESSION"

#endif