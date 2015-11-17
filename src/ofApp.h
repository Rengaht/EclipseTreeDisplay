#pragma once

#include <thread>
#include "ofMain.h"
#include "HttpFormManager.h"
#include "ofxHttpUtils.h"

#include "ATree.h"
#include "AClockNumber.h"



class ofApp : public ofBaseApp{

	public:

		enum ShowType {S_KV,S_TREE,S_BT,S_NO};
		enum RequestType {ASK_TIME,ASK_SCHEDULE,ASK_OLD_TREE,ASK_NEW_TREE};

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void onTreeFinished(int & e);
		void onTreeInFinished(int & e);
		
		vector<ATree> _vec_tree;
		int _cur_tree_index,_last_tree_index;

		float _dmillis,_last_millis;
		bool tree_in_finished;

		map<int,ShowType> _map_schedule;

		void sendThreadRequset(RequestType rt);
		void sendHttpRequest(RequestType rt);
		
		
		bool _init_request_finished;

		ofxHttpUtils httpUtils;
		void httpResponse(ofxHttpResponse &response);


		void playTree(float tree_due);
		void playTree();
		void goNextShow();
		ShowType last_show,cur_show;

		bool _is_asking_tree;
		bool _is_playing_tree;

		float _time_btw_show;		
		void startBtwShowTimer(int now_second_code);
		void updateBtwShowTimer();

		void findNextShow(ShowType& type, float& due_time);
		void exit();

		AClockNumber _clock_num;

		void sendSlackLog(string message);

	private:
		void insertNewTree(ATree tree);
		
		int _index_kv;
		FrameAnimation _anim_change_show;

		void readSavedInitData();

		int _cur_show_starttime;
};
