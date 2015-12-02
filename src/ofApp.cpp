#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	_UpdateWithNetwork=true;

	GlobalParam::GetInstance()->loadParameter();
	

	ATreeImage::GetInstance()->loadImage();
	ATreeImage::GetInstance()->_mov_tree_bg.play();


	sendSlackLog("[Info] Program Start!");
	
	string timestamp=ofGetTimestampString("%y%m%e%H%M");
	//ofLogToFile("log/SLog_"+timestamp+".txt", true);


	/*for(int i=0;i<10;++i){
		ATree *tree=new ATree();
		_vec_tree.push_back(*tree);
		
	}*/
	ofAddListener(ATree::event_finish,this,&ofApp::onTreeFinished);
	ofAddListener(ATree::event_in_finish,this,&ofApp::onTreeInFinished);

	_cur_tree_index=-1;
	_last_tree_index=-1;
	
	
	
	//_http_manager.setVerbose(true);
	//ofAddListener(_http_manager.formResponseEvent,this,&ofApp::httpResponse);
	ofAddListener(httpUtils.newResponseEvent, this, &ofApp::httpResponse);
	httpUtils.start();
	httpUtils.setTimeoutSeconds(5);
	
	_time_btw_show=0;

	//_init_request_finished=false;

	
	//sendHttpRequest(ASK_TIME);
	sendThreadRequset(ASK_TIME);
	_request_status=RequestStatus::ASKING_TIME;

	_index_kv=3;
	_anim_change_show=FrameAnimation(2000);
	_anim_change_show.Restart();


	//_is_playing_tree=false;
	
	_anim_sound_change=FrameAnimation(2000);
	_playing_bgm=false;
	//startBGM();


	ofSetWindowTitle("_EclipseTreeDisplay");
	ofBackground(ofColor::black);

	ofHideCursor();
	ofSetFullscreen(true);
}

//--------------------------------------------------------------
void ofApp::update(){

	_dmillis=ofGetElapsedTimeMillis()-_last_millis;
	_last_millis+=_dmillis;

	/*if(_cur_tree_index>-1) _vec_tree[_cur_tree_index].update(_dmillis);
	if(_last_tree_index>-1) _vec_tree[_last_tree_index].update(_dmillis);*/
	if(_cur_tree_index>-1) _cur_tree.update(_dmillis);
	if(_last_tree_index>-1) _last_tree.update(_dmillis);
	
	_anim_change_show.Update(_dmillis);
	_clock_num.upate(_dmillis);

	ATreeImage::GetInstance()->_mov_tree_bg.update();
	ATreeImage::GetInstance()->_mov_bt.update();	
	
	ATreeImage::GetInstance()->_mov_kv[_index_kv].update();	
	

	ofSoundUpdate();
	_anim_sound_change.Update(_dmillis);
	if(_playing_bgm){
		ATreeImage::GetInstance()->_sound_bgm.setVolume(_anim_sound_change.GetPortion());
	}else{
		ATreeImage::GetInstance()->_sound_bgm.setVolume(1-_anim_sound_change.GetPortion());
	}
	

	switch(cur_show){
		case S_BT:		
			if(ATreeImage::GetInstance()->_mov_bt.getIsMovieDone() || _time_btw_show<-1000){
				//ATreeImage::GetInstance()->_mov_bt.setFrame(0);
				//ATreeImage::GetInstance()->_mov_bt.stop();
				goNextShow();
			}
			if(_time_btw_show<=GlobalParam::GetInstance()->_bt_time-GlobalParam::GetInstance()->_clock_end_time) _clock_num.setVisible(false);
			else{
				if(_time_btw_show<=GlobalParam::GetInstance()->_bt_time-GlobalParam::GetInstance()->_clock_start_time) _clock_num.setVisible(true);
			}
			break;
		case S_KV:			
			if(ATreeImage::GetInstance()->_mov_kv[_index_kv].getIsMovieDone() || _time_btw_show<-1000){
				//ATreeImage::GetInstance()->_mov_kv[_index_kv].setFrame(0);
				//ATreeImage::GetInstance()->_mov_kv[_index_kv].stop();
				goNextShow();
			}
			break;
		case S_TREE:
			if(_request_status==RequestStatus::GET_TREE && _time_btw_show<=0){
				ofLog()<<ofGetTimestampString()<<"Tree Time Finish At "<<ofToString(GlobalParam::GetInstance()->getCalibTimeCode());
				
				tree_in_finished=false;
				_cur_tree.reset();

				_last_tree_index=_cur_tree_index;			
				_last_tree=ATree(_cur_tree);

				

				goNextShow();
				//_is_playing_tree=false;
			}
			break;

	}
	if(_request_status>=RequestStatus::GET_INIT_DATA)
			_time_btw_show-=_dmillis;	
			
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetBackgroundColor(ofColor::black);
	if(_request_status<RequestStatus::GET_INIT_DATA) return;

	float tchange=_anim_change_show.GetPortion();
	if(tchange<1 || !tree_in_finished){
		ofPushStyle();
		//ofSetColor(255,(1-tchange)*255);
		switch(last_show){
			case S_TREE:
				//if(_last_tree_index>-1) _vec_tree[_last_tree_index].draw();		
				if(_last_tree_index>-1) _last_tree.draw();		
				break;
			case S_KV:
				ATreeImage::GetInstance()->_mov_kv[_index_kv].draw(0,0);
				break;
			case S_BT:
				ATreeImage::GetInstance()->_mov_bt.draw(0,0);
				_clock_num.draw();
				break;
		}
		ofPopStyle();
	}

	ofPushStyle();
	
	switch(cur_show){
		case S_TREE:			
			//if(last_show==ShowType::S_TREE && _last_tree_index>-1 && !tree_in_finished)_vec_tree[_last_tree_index].draw();		
			//if(_request_status==RequestStatus::GET_TREE && _cur_tree_index>-1) _vec_tree[_cur_tree_index].draw();
			if(_request_status==RequestStatus::GET_TREE && _cur_tree_index>-1){
				_cur_tree.draw();
			}else{
				cout<<ofToString(_request_status)<<" tree# "<<ofToString(_cur_tree_index)<<endl;
			}
			
			break;
		case S_KV:
			ofSetColor(255,tchange*255);
			ATreeImage::GetInstance()->_mov_kv[_index_kv].draw(0,0);
			break;
		case S_BT:
			ofSetColor(255,tchange*255);
			ATreeImage::GetInstance()->_mov_bt.draw(0,0);
			_clock_num.draw();
			break;
	}
	ofPopStyle();

	


}

void ofApp::playTree(){
	playTree(GlobalParam::GetInstance()->_tree_time,(_cur_tree_index+1)%_vec_tree.size());
}

void ofApp::playTree(float tree_due,int tree_index){
	
	//if(_cur_tree_index>-1 && !_vec_tree[_cur_tree_index].display_finished) return;
	/*if(_is_playing_tree) return;
	_is_playing_tree=true;
	_is_asking_tree=false;*/

	//_last_tree_index=_cur_tree_index;	
	//_cur_tree_index=(_cur_tree_index+1)%_vec_tree.size();

	_cur_tree=ATree(_vec_tree[tree_index]);

	if(tree_index>-1){
		//_vec_tree[_cur_tree_index].start(tree_due);
		_cur_tree.start(tree_due);
		_cur_tree_index=tree_index;

		ofLog()<<ofGetTimestampString()<<"Tree "<<_cur_tree_index
			<<" Started At "<<ofToString(GlobalParam::GetInstance()->getCalibTimeCode())<<" due= "<<ofToString(tree_due);
	}

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	//ofLog()<<"Get Key"<<char(key);

	switch(key){
		/*case 'a':
			playTree();
			break;*/
		case 'f':
			ofToggleFullscreen();
			break;
		/*case 'c':
			_clock_num.toggleShow();
			break;*/
		case 'b':
			if(_playing_bgm) pauseBGM();
			else startBGM();
			break;
		case 'n':
			_UpdateWithNetwork=false;
			ofLog()<<"Set _UpdateWithNetwork=false!";
			sendSlackLog("Set _UpdateWithNetwork=false!");
			break;
		case 'N':
			_UpdateWithNetwork=true;
			ofLog()<<"Set _UpdateWithNetwork=true!";
			sendSlackLog("Set _UpdateWithNetwork=true!");
			break;


			/*case 's':
			sendHttpRequest(ASK_SCHEDULE);
			break;
		case 't':
			sendHttpRequest(ASK_TIME);
			break;
		case 'n':
			sendHttpRequest(ASK_NEW_TREE);
			break;
		case 'o':
			sendHttpRequest(ASK_OLD_TREE);
			break;*/

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::onTreeFinished(int& e){

	
	ofLog()<<ofGetTimestampString()<<"Tree Finished At "<<ofToString(GlobalParam::GetInstance()->getCalibTimeCode())
			<<" starttime= "<<ofToString(e);
	
	/*tree_in_finished=false;
	_cur_tree.reset();*/


	//_is_playing_tree=false;
	

}

void ofApp::onTreeInFinished(int& e){

	tree_in_finished=true;
	
	// look for next tree
	//sendThreadRequset(ASK_NEW_TREE);

}

void ofApp::sendThreadRequset(RequestType rt){
	/*thread rthread(&ofApp::sendHttpRequest,this,rt);
	rthread.join();*/
	sendHttpRequest(rt);

}

void ofApp::sendHttpRequest(RequestType rt){


	ofxHttpForm f;
	
	switch(rt){
		case ASK_TIME:			
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getTimeset";
			_request_status=RequestStatus::ASKING_TIME;				
			break;
		case ASK_SCHEDULE:
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getTimetable";
			_request_status=RequestStatus::ASKING_SCHEDULE;				
			break;
		case ASK_OLD_TREE:
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getOldTree&mid="+ofToString(GlobalParam::GetInstance()->_machine_id);
			_request_status=RequestStatus::ASKING_OLD_TREE;				
			break;
		case ASK_NEW_TREE:
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getNewTree&mid="+ofToString(GlobalParam::GetInstance()->_machine_id);
			_request_status=RequestStatus::ASKING_TREE;	
			ofLog()<<"Ask new tree...";
			break;
	}

	if(_UpdateWithNetwork) httpUtils.addForm(f);
	else{
		if(rt==ASK_NEW_TREE){
			_request_status=RequestStatus::GET_TREE;
			//_cur_tree_index=(_cur_tree_index+1)%_vec_tree.size();
				
			playTree(_time_btw_show,(_cur_tree_index+1)%_vec_tree.size());
		}
	}
	
}
//void ofApp::httpResponse(HttpFormResponse &response){
void ofApp::httpResponse(ofxHttpResponse &response){
	
	//ofLog()<<ofToString(response.status) + ": " + (string)response.responseBody;
	
	if(response.status!=200){ // timeout fail
		switch(_request_status){
			case ASKING_TIME: // cannot get init-data
				httpUtils.clearQueue();
				sendSlackLog("[ERR] Cannot get time!");
			
				readSavedInitData(RequestType::ASK_TIME);
				sendHttpRequest(RequestType::ASK_SCHEDULE);
				break;
			case ASKING_SCHEDULE:
				httpUtils.clearQueue();
				sendSlackLog("[ERR] Cannot get schedule!");
				
				readSavedInitData(RequestType::ASK_SCHEDULE);
				sendHttpRequest(RequestType::ASK_OLD_TREE);
				break;
			case ASKING_OLD_TREE:
				httpUtils.clearQueue();
				sendSlackLog("[ERR] Cannot get old trees!");
				
				readSavedInitData(RequestType::ASK_OLD_TREE);				
				_request_status=RequestStatus::GET_INIT_DATA;
				goNextShow();
				break;
			case ASKING_TREE:
				httpUtils.clearQueue();
				sendSlackLog("[ERR] Cannot get new tree");

				_request_status=RequestStatus::GET_TREE;
				
				playTree(_time_btw_show,(_cur_tree_index+1)%_vec_tree.size());
				break;
			
		}
		
		return;
	}




	Json::Value root;  
	Json::Reader reader;
	bool parse_success=reader.parse(response.responseBody,root);
	if(!parse_success){
		if(response.responseBody.getText()=="ok") return;

		ofLog()<<"Failed to parse configuration\n"
				<<reader.getFormatedErrorMessages();

		sendSlackLog("[ERR] Cannot parse Json: "+ofToString(response.responseBody)+" \n "+reader.getFormatedErrorMessages());

		return;
	}


	string rtype=root["name"].asString();
	if(rtype.compare("getTimeset")==0){
		
		GlobalParam::GetInstance()->_kv_time=ofToInt(root["KV_T"].asString())*1000;
		GlobalParam::GetInstance()->_tree_time=ofToInt(root["TREE_T"].asString())*1000;
		GlobalParam::GetInstance()->_bt_time=ofToInt(root["BT_T"].asString())*1000;
		GlobalParam::GetInstance()->_delta_second_to_server=ofToInt(root["S_T"].asString())-GlobalParam::GetInstance()->getTimeCode();

		Json::Value val;
		val["KV_T"]=root["KV_T"].asString();
		val["BT_T"]=root["BT_T"].asString();
		val["TREE_T"]=root["TREE_T"].asString();
		val["S_T"]=root["S_T"].asString();

		GlobalParam::GetInstance()->writeJson(val,GlobalParam::TimeSetFile);

		//sendHttpRequest(ASK_SCHEDULE);
		sendThreadRequset(ASK_SCHEDULE);

	}else if(rtype.compare("getTimetable")==0){

		_map_schedule.clear();
		_kv_order.clear();

		Json::Value result=root["table"];
		for(Json::ValueIterator itr=result.begin();itr!=result.end();itr++){			
			ShowType type= (ShowType)ofToInt((*itr)[Json::UInt(1)].asString());
			int time=ofToInt((*itr)[Json::UInt(0)].asString());
			_map_schedule.insert(pair<int,ShowType>(time,type));
			if(type==ShowType::S_KV) _kv_order.push_back(time);
		}
		Json::Value val;
		val["table"]=result;
		GlobalParam::GetInstance()->writeJson(val,GlobalParam::TimeTableFile);


		//sendHttpRequest(ASK_OLD_TREE);
		sendThreadRequset(ASK_OLD_TREE);

	}else if(rtype.compare("getNewTree")==0){
		
		int result=root["result"].asInt();
		if(result==1){
			
			Json::Value atree=root["tree"];
			//for(Json::ValueIterator itr=atree.begin();itr!=atree.end();itr++){				
			ATree t=ATree(atree["TreeCode"].asString(),atree["FromName"].asString(),atree["ToName"].asString(),atree["Message"].asString(),atree["TID"].asString());
			insertNewTree(t);
			_request_status=RequestStatus::GET_TREE;
			playTree(_time_btw_show,0);		

			ofLog()<<"Get New Tree "<<response.responseBody.getText();
			sendSlackLog("[Info] Get New Tree: ("+atree["FromName"].asString()+" -> "+atree["ToName"].asString()+")");
		}else{
			//_cur_tree_index=(_cur_tree_index+1)%_vec_tree.size();
			ofLog()<<"There's no New Tree!";
			_request_status=RequestStatus::GET_TREE;
			playTree(_time_btw_show,(_cur_tree_index+1)%_vec_tree.size());		
		}
		
		

	}else if(rtype.compare("getOldTree")==0){
		
		int result=root["result"].asInt();
		if(result==1){
			Json::Value atree=root["tree"];
			for(Json::ValueIterator itr=atree.begin();itr!=atree.end();itr++){				
				ATree t=ATree((*itr)["TreeCode"].asString(),(*itr)["FromName"].asString(),(*itr)["ToName"].asString(),(*itr)["Message"].asString(),(*itr)["TID"].asString());
				insertNewTree(t);
			}
			Json::Value val;
			val["tree"]=atree;
			GlobalParam::GetInstance()->writeJson(val,GlobalParam::OldTreeFile);

		}
		checkOldTreeFill();

		//_init_request_finished=true;
		_request_status=RequestStatus::GET_INIT_DATA;
		goNextShow();

	}



}

void ofApp::checkOldTreeFill(){
	int c=_vec_tree.size();
	if(c<1){
		ofLog()<<"[ERR] There's no tree at initiation!!!";
		sendSlackLog("[ERR] There's no tree at initiation!!!");
		ATree dtree;
		_vec_tree.push_back(dtree);
		c=1;
	}

	/*int s=0;	
	while(_vec_tree.size()<GlobalParam::GetInstance()->_max_tree){
		
		ATree t=_vec_tree[s];
		ATree copy_tree=ATree(t._arr_part_index[0],t._arr_part_index[1],t._arr_part_index[2],t._arr_part_index[3],t._arr_part_index[4],
			t._arr_word[0],t._arr_word[2],t._arr_word[1],t._arr_word[3]);
		_vec_tree.push_back(copy_tree);
		

		s=(s+1)%c;
	}*/

}


void ofApp::insertNewTree(ATree tree){

	if(_vec_tree.size()<GlobalParam::GetInstance()->_max_tree){
		//_vec_tree.push_back(tree);
		_vec_tree.insert(_vec_tree.begin(),tree);
		//_cur_tree_index=0;
	}else{
		/*vector<ATree> tmp;
		tmp.push_back(tree);*/

		_vec_tree.erase(_vec_tree.end()-1);
		_vec_tree.insert(_vec_tree.begin(),tree);
		//_cur_tree_index=0;
		
		//int s=_vec_tree.size();
		//for(int i=0;i<s;++i){
		//	int p=(_cur_tree_index-i+s)%s;
		//	
		//	if(i==0) continue;
		//	
		//	if(i==s-1){
		//		_vec_tree[p]=tree;
		//		ofLog()<<"Tree insert at "<<p;
		//	}else{
		//		_vec_tree[p]=_vec_tree[(p-1+s)%s];
		//		//ofLog()<<"Tree swap "<<p<<" <- "<<(p-1+s)%s;
		//	}

		//}
		
	}

}

void ofApp::goNextShow(){

	ShowType next_show;
	float show_due;
	findNextShow(next_show,show_due);
	
	_time_btw_show=show_due;

	
	
	switch(next_show){
		case S_KV:
			
			//_index_kv=(_index_kv+1)%4;

			ATreeImage::GetInstance()->_mov_kv[_index_kv].play();
			ATreeImage::GetInstance()->_mov_kv[_index_kv].setPosition(1-show_due/GlobalParam::GetInstance()->_kv_time);
			//if(show_due>=GlobalParam::GetInstance()->_kv_time)	ATreeImage::GetInstance()->_mov_kv[_index_kv].play();			
			//else ofLog()<<"Not enough time to play kv: "<<show_due;
			startBGM();
			break;

		case S_BT:
			ATreeImage::GetInstance()->_mov_bt.play();
			ATreeImage::GetInstance()->_mov_bt.setPosition(1-show_due/GlobalParam::GetInstance()->_bt_time);
					
			//if(show_due>=GlobalParam::GetInstance()->_bt_time)	ATreeImage::GetInstance()->_mov_bt.play();
			//else ofLog()<<"Not enough time to play bt: "<<show_due;
			pauseBGM();

			break;

		case S_TREE:
			
			
			sendHttpRequest(ASK_NEW_TREE);
			
			startBGM();
			break;		
	}
	last_show=cur_show;
	cur_show=next_show;

	if(last_show!=cur_show){
		_anim_change_show.Restart();
		//_last_tree_index=-1;		
		//if(last_show!=ShowType::S_TREE && cur_show==ShowType::S_TREE) _cur_tree_index=-1;
	}
}


void ofApp::startBtwShowTimer(int now_code){

	int a=4000;
	for(auto pair:_map_schedule){
		if(pair.first>now_code && now_code>a){
			_time_btw_show=(pair.first-now_code)*1000.0;					
			ofLog()<<"Now "<<now_code<<" Wait "<<_time_btw_show;
			return;
		}
		a=pair.first;
	}

	
}

void ofApp::findNextShow(ShowType& type, float& due_time){
	int now_code=GlobalParam::GetInstance()->getCalibTimeCode();
	ofLog()<<ofGetTimestampString()<<"Find Show at "<<now_code;

	auto itr=_map_schedule.find(now_code);
	if(itr==_map_schedule.end()){
		int a=0;
		ShowType t;
		for(auto pair:_map_schedule){
			if(pair.first>now_code && now_code>a){
				due_time=(pair.first-now_code)*1000.0;		
				if(due_time<0) due_time=10;

				type=t;
				ofLog()<<"Get Nearest "<<ofToString(t)<<" at "<<a<<" to "<<pair.first<<" due= "<<due_time;
				_cur_show_starttime=pair.first;

				if(type==ShowType::S_KV){
					_index_kv=(find(_kv_order.begin(),_kv_order.end(),pair.first)-_kv_order.begin())%4;				
					ofLog()<<">>>  kv "<<_index_kv;
				}

				break;
			}
			a=pair.first;
			t=pair.second;
		}
	}else{
		type=_map_schedule[now_code];
		_cur_show_starttime=now_code;
		ofLog()<<"Get Show "<<ofToString(type)<<" at "<<now_code;
				
		switch(type){
			case S_KV:
				due_time=GlobalParam::GetInstance()->_kv_time;
				_index_kv=(find(_kv_order.begin(),_kv_order.end(),now_code)-_kv_order.begin())%4;
				ofLog()<<">>>  kv "<<_index_kv;
				break;
			case S_BT:
				due_time=GlobalParam::GetInstance()->_bt_time;
				break;
			case S_TREE:
				due_time=GlobalParam::GetInstance()->_tree_time;
				break;		
		}
	}


}


void ofApp::exit(){
	
	sendSlackLog("[Info] Program Exit!");

	httpUtils.clearQueue();
	httpUtils.stop();

}


void ofApp::readSavedInitData(RequestType type){
	
	Json::Value file_val;
	Json::Value atree;
	Json::Value table;

	switch(type){
		case ASK_TIME:
			file_val=GlobalParam::GetInstance()->parseFileToJson(GlobalParam::GetInstance()->TimeSetFile);

			GlobalParam::GetInstance()->_kv_time=ofToInt(file_val["KV_T"].asString())*1000;
			GlobalParam::GetInstance()->_tree_time=ofToInt(file_val["TREE_T"].asString())*1000;
			GlobalParam::GetInstance()->_bt_time=ofToInt(file_val["BT_T"].asString())*1000;
			GlobalParam::GetInstance()->_delta_second_to_server=0;//ofToInt(timeset["S_T"].asString())-GlobalParam::GetInstance()->getTimeCode();
			break;
		case ASK_SCHEDULE:
			_kv_order.clear();
			_map_schedule.clear();

			file_val=GlobalParam::GetInstance()->parseFileToJson(GlobalParam::GetInstance()->TimeTableFile);
			table=file_val["table"];
			for(Json::ValueIterator itr=table.begin();itr!=table.end();itr++){			
				ShowType type= (ShowType)ofToInt((*itr)[Json::UInt(1)].asString());
				int time=ofToInt((*itr)[Json::UInt(0)].asString());
				_map_schedule.insert(pair<int,ShowType>(time,type));
				if(type==ShowType::S_KV) _kv_order.push_back(time);

			}
			break;
		case ASK_OLD_TREE:
			file_val=GlobalParam::GetInstance()->parseFileToJson(GlobalParam::GetInstance()->OldTreeFile);
			atree=file_val["tree"];
			for(Json::ValueIterator itr=atree.begin();itr!=atree.end();itr++){				
				ATree t=ATree((*itr)["TreeCode"].asString(),(*itr)["FromName"].asString(),(*itr)["ToName"].asString(),(*itr)["Message"].asString(),(*itr)["TID"].asString());
				insertNewTree(t);
			}
			break;
	}
}

void ofApp::sendSlackLog(string message){
	
	ofLog()<<ofGetTimestampString()<<message;

	ofxHttpForm f;
	f.action="https://hooks.slack.com/services/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";
	f.method=OFX_HTTP_POST;
	/*string text_str="{\"text\":\""+content+"\"}";
	f.addFormField("payload",text_str);*/

	Json::Value slack_data;
	slack_data["text"]="#MACHINE_"+ofToString(GlobalParam::GetInstance()->_machine_id)+"#  "+message;

	f.addFormField("payload",slack_data.toStyledString());

	httpUtils.addForm(f);

}

#pragma region HANDLE_SOUND

void ofApp::startBGM(){

	if(_playing_bgm) return;

	_anim_sound_change.Restart();
	_playing_bgm=true;

	ATreeImage::GetInstance()->_sound_bgm.setLoop(true);
	ATreeImage::GetInstance()->_sound_bgm.play();
	ATreeImage::GetInstance()->_sound_bgm.setVolume(0);
}

void ofApp::pauseBGM(){
	if(!_playing_bgm) return;

	_anim_sound_change.Restart();
	_playing_bgm=false;

	
}

#pragma endregion