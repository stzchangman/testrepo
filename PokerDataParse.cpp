// PokerDataParse.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
//#define _CRT_SECURE_NO_WARNINGS
//#pragma warning(disable:4996)

#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
using namespace std;


#define TIME_UNIT 16  // 16 : 분단위 , 19:초단위 

struct Info {
	void init() {
		channel_id = 0;
		user_id = 0;
		for (int i = 0; i < 10; i++) type[i] = 0;
		for (int i = 0; i < 50; i++) strid[i] = 0;
		for (int i = 0; i < 5; i++) player[i] = 0;
	}

	Info() {
		init();
	}
	int channel_id;
	char type[10];
	char strid[50];
	int player[5];
	int user_id;

	void setType(char* str) {
		strcpy(type, str);
	}

	void setid(char* str) {
		strcpy(strid, str);
	}

	int addUser(int userid) {
		for (int i = 0; i < 5; i++) {
			if (player[i] == userid)
				return 0;
		}
		for (int i = 0; i < 5; i++) {
			if (player[i] == 0) {
				player[i] = userid;
				return 1;
			}
		}
		return 0;
	}
	int removeUser(int userid) {
		for (int i = 0; i < 5; i++) {
			if (player[i] == userid) {
				player[i] = 0;
				return 1;
			}
		}
		return 0;
	}
	int getCount() {
		int cnt = 0;
		for (int i = 0; i < 5; i++) {
			if (player[i] > 0) cnt++;
		}
		return cnt;
	}

	void print() {
		printf("===========> %d %s %s \n", user_id,strid, type);
	}
};



struct Room {
public:
	Room() { for (int i = 0; i < 5; i++) player[i] = 0; channel_id = 0;  }
	int channel_id;
	string strid;
	int player[5];

	int isRoom(char* str) {
		if (strid.length() == 0) return 1;
		return 0;
	}

	void addUsers(int arr[]) {
		for (int i = 0; i < 5; i++) {
			player[i] = arr[i];
		}
	}

	int addUser(int userid) {
		for (int i = 0; i < 5; i++) {
			if (player[i] == userid)
				return 0;
		}
		for (int i = 0; i < 5; i++) {
			if (player[i] == 0) {
				player[i] = userid;
				return 1;
			}
		}
		return 0;
	}
	int removeUser(int userid) {
		for (int i = 0; i < 5; i++) {
			if (player[i] == userid) {
				player[i] = 0;
				return 1;
			}
		}
		return 0;
	}
	int getCount() {
		int cnt = 0;
		for (int i = 0; i < 5; i++) {
			if (player[i] > 0) cnt++;
		}
		return cnt;
	}

	void print() {
		printf("===========>  %d %s %d \n",  channel_id,strid.c_str(),getCount());
	}

};

#define ROOM_TOTAL 1000
class RoomManager {
	map<string, Room> tmap;

public:
	void DeleteUser(int user_id) {
		map<string, Room>::iterator iter;
		for (iter = tmap.begin(); iter != tmap.end(); ++iter) {
			iter->second.removeUser(user_id);
		}
		for (iter = tmap.begin(); iter != tmap.end(); ++iter) {
			int ret = iter->second.removeUser(user_id);
			if (ret == 1 && iter->second.getCount() == 0) {
				tmap.erase(iter->first);
				break;
			}
		}
	}

	void RefreshRoom(Info* info) {
		if (strcmp(info->type, "quit") == 0) {
			DeleteUser(info->user_id);
		}
		else
		{
			Room tmp;
			tmp.channel_id = info->channel_id;
			tmp.strid = info->strid;
			tmp.addUsers(info->player);

			tmap[info->strid] = tmp;
		}
	}

	void print(FILE* fp, char* strdate, int header = 0) {
		fprintf(fp,"%s,", strdate);

		int arr[] = { 101,102,103,104,105,106,107,108,109,201,202,203,204,205,206,207,208,209,301,302,303,304,305,306,307,308,401,402,403,404  };
		int arrSize = sizeof(arr) / sizeof(int);
		string str;
		char buf[255];
		map<string, int> room;
		for (int i = 0; i < arrSize; i++) {
			for (int j = 1; j <= 5; j++) {
				sprintf(buf, "%d_%d", arr[i], j);
				str = buf;
				room[str] = 0;

			}
		}

		if (header == 1) {
			for (int i = 0; i < arrSize; i++) 
				for (int j = 1; j <= 5; j++) 
					fprintf(fp,"%d_%d,", arr[i], j);				
			fprintf(fp,"\n");
			return;
		}

		map<string, int>::iterator iter2;

		// printf("sizeof %d \n", sizeof(arr)/sizeof(int));
		//for(int i=0;i<sizeof(arr);i++)
		map<string, Room>::iterator iter;
		for (iter = tmap.begin(); iter != tmap.end(); ++iter) {
			sprintf(buf,"%d_%d", iter->second.channel_id, iter->second.getCount());
			iter2 = room.find(buf);
			if (iter2 != room.end()) iter2->second += 1;
		}

		for (iter2 = room.begin(); iter2 != room.end(); ++iter2) {
			fprintf(fp,"%d,", iter2->second);
		}
		fprintf(fp,"\n");

	}

};

class Poker {
	RoomManager manager;
	FILE* fp2;
	int m_count = 0;
public:
	Poker() {		
		memset(m_strDate, 0, 50);
	}
	~Poker() {
	}

	char m_strDate[50];

	void updateDate(char* strDate) {
		if (strcmp(m_strDate, strDate) != 0) {
			if(strlen(m_strDate) == 0)
				manager.print(fp2,(char*)"Date",1);
			else 
				manager.print(fp2,m_strDate);
			m_count++;
			if (m_count % 100 == 0) printf("%s count : %d\n", m_strDate, m_count);
			strcpy(m_strDate, strDate);
		}
	}

	void _nextToken(char* buf, char* strInfo) {
		int nsize = strlen(strInfo);
		for (int i = 0; i < 256; i++) buf[i] = 0;
		int start, end;
		start = -1;
		end = -1;

		for (int i = 0; i < nsize; i++) {
			if (*(strInfo + i) == '\"') {
				if (start == -1)
					start = i+1;
				else {
					end = i;
					break;
				}
			}
		}

		if (start >= 0 && end >= 0) {
			strncpy(buf, strInfo + start, end - start);
			// printf("Parse : [%s] \n", buf);
		}

	}

	int _nextTokenNumber(char* strInfo) {
		int nsize = strlen(strInfo);
		char buf[256] = { 0, };
		int start, end;
		start = -1;
		end = -1;

		for (int i = 0; i < nsize; i++) {
			if (*(strInfo + i) >= '0' && *(strInfo + i) <= '9') {
				if (start == -1)
					start = i;
				else {
					end = i+1;
				}
			}
			else {
				if (end >= 0)
					break;
			}
		}

		if (start >= 0 && end >= 0) {
			strncpy(buf, strInfo + start, end - start);
			//printf("Parse : [%s] \n", buf);
		}
		if (strlen(buf) == 0) return 0;
		return atoi(buf);
	}
	
	void _nextTokenList(Info* info, char* strInfo) {
		int nsize = strlen(strInfo);
		char buf[256] = { 0, };
		int start, end;
		start = -1;
		end = -1;

		for (int i = 0; i < nsize; i++) {
			if (*(strInfo + i) == '{') {
				start = i+1;
			}
			if (*(strInfo + i) == '}') {
				end = i;
				break;
			}
		}

		if (start >= 0 && end >= 0) {
			strncpy(buf, strInfo + start, end - start);

			char* result;
			result = strtok(buf, ",");
			while (result != NULL) {
				//printf("%s\n", result);
				int nsize = strlen(result);
				for (int i = 0; i < nsize; i++) {
					if (result[i] == ':') {
						int ret = _nextTokenNumber(result + i);
						//printf("user:%d\n", ret);
						info->addUser(ret);
						break;
					}
				}
				result = strtok(NULL, ",");
			}
			// printf("Parse : [%s] \n", buf);
		}
	}

	void parse(Info* info,char* strInfo) {
		const char* strType = "\"type\"";
		const char* strId = "\"id\"";
		const char* strChannel = "\"channel_id\"";
		const char* strPlayers = "\"players\"";
		char buf[512] = { 0, };

		char* p = strstr(strInfo, strType);
		if (p) {
			_nextToken(buf, p + strlen(strType));
			info->setType(buf);
			//printf("parse : %s \n", p+strlen(strType));
		}

		p = strstr(strInfo, strId);
		if (p) {
			_nextToken(buf, p + strlen(strId));
			info->setid(buf);
			//printf("parse : %s \n", p + strlen(strId));
		}

		p = strstr(strInfo, strChannel);
		if (p) {
			info->channel_id = _nextTokenNumber(p + strlen(strChannel));
			
			//printf("parse : %s \n", p + strlen(strChannel));
		}

		p = strstr(strInfo, strPlayers);
		if (p) {
			_nextTokenList(info,p + strlen(strPlayers));
			//printf("parse : %s \n", p + strlen(strChannel));
		}
	}

	void updateInfo(Info* info) {
		manager.RefreshRoom(info);
	}

	void processLine(char* strDate, int nUser, char* strInfo) {
		if (nUser == 0) return;
		// printf("=========processLine : %s : %d : %s \n", strDate, nUser, strInfo);

		updateDate(strDate);

		Info info;
		info.user_id = nUser;
		parse(&info, strInfo);

		updateInfo(&info);
		
	}

	void readline(char* buf) {
		char strdate[30] = { 0, };
		char strUser[20] = { 0, };
		char strInfo[1024] = { 0, };

		int cnt = 0;
		char* ptr = strtok(buf, "\t");
		while (ptr != NULL) {
			if (cnt == 0 && strnlen(ptr, 30) > 10) strncpy(strdate, ptr, TIME_UNIT);
			if (cnt == 1 && strnlen(ptr, 30) > 1) strcpy(strUser, ptr);
			if (cnt == 2 && strnlen(ptr, 30) > 10) strcpy(strInfo, ptr);
			cnt++;
			ptr = strtok(NULL, "\t");
		}

		if (strlen(strdate) > 10 && strlen(strUser) > 1 && strlen(strInfo) > 10)
			processLine(strdate, atoi(strUser), strInfo);


	}

public:
	void process(char* strInput,char* strOutput) {
		FILE* fp;
		
		char buf[1024] = { 0, };
		//fp2 = fopen("d:\\tmp\\result.txt", "w");
		//fp = fopen("d:\\vitual\\poker_log.txt", "r");
		fp2 = fopen(strOutput, "w");
		fp = fopen(strInput, "r");
		if (fp == NULL || fp2 == NULL) {
			printf("Error Open File\n");
			return;
		}


		while (fgets(buf, 1000, fp)) {
			//printf(buf);
			readline(buf);
		}
		fclose(fp);
		if (fp2) fclose(fp);

	}
};

void process() {
}

int main(int argc,char* argv[])
{
	if (argc < 3) {
		printf("Usage : PokerDataParse src desc\n");
		return 0;
	}

	Poker poker;
	poker.process(argv[1],argv[2]);

    return 0;
}

