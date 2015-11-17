#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	

	ATreeImage::GetInstance()->loadImage();
	ATreeImage::GetInstance()->_mov_tree_bg.play();


	GlobalParam::GetInstance()->loadParameter();
	sendSlackLog("[Info] Program Start!");

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
	httpUtils.setTimeoutSeconds(3);
	
	_time_btw_show=0;

	_init_request_finished=false;

	
	//sendHttpRequest(ASK_TIME);
	sendThreadRequset(ASK_TIME);

	_index_kv=3;
	_anim_change_show=FrameAnimation(2000);
	_anim_change_show.Restart();


	_is_playing_tree=false;

}

//--------------------------------------------------------------
void ofApp::update(){

	_dmillis=ofGetElapsedTimeMillis()-_last_millis;
	_last_millis+=_dmillis;

	if(_cur_tree_index>-1)_vec_tree[_cur_tree_index].update(_dmillis);
	if(_last_tree_index>-1)_vec_tree[_last_tree_index].update(_dmillis);
	
	_anim_change_show.Update(_dmillis);
	_clock_num.upate(_dmillis);

	ATreeImage::GetInstance()->_mov_tree_bg.update();
	ATreeImage::GetInstance()->_mov_bt.update();	
	
	ATreeImage::GetInstance()->_mov_kv[_index_kv].update();	
	
	

	switch(cur_show){
		case S_BT:		
			if(ATreeImage::GetInstance()->_mov_bt.getIsMovieDone() || _time_btw_show<-1000){
				//ATreeImage::GetInstance()->_mov_bt.setFrame(0);
				//ATreeImage::GetInstance()->_mov_bt.stop();
				goNextShow();
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
			if(_is_playing_tree && _time_btw_show<=0){
				ofLog()<<"Tree Time Finish At "<<ofToString(GlobalParam::GetInstance()->getCalibTimeCode());
				goNextShow();
				_is_playing_tree=false;
			}
			break;

	}
	if(_init_request_finished)
			_time_btw_show-=_dmillis;	
			
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	
	ofSetBackgroundColor(ofColor::black);
	if(!_init_request_finished) return;

	float tchange=_anim_change_show.GetPortion();
	if(tchange<1){
		ofPushStyle();
		ofSetColor(255,(1-tchange)*255);
		switch(last_show){
			case S_TREE:
				if(_cur_tree_index>-1) _vec_tree[_cur_tree_index].draw();
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
	ofSetColor(255,tchange*255);
	switch(cur_show){
		case S_TREE:			
			if(_last_tree_index>-1 && !tree_in_finished)_vec_tree[_last_tree_index].draw();		
			if(!_is_asking_tree && _cur_tree_index>-1) _vec_tree[_cur_tree_index].draw();
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

	_clock_num.draw();


}

void ofApp::playTree(){
	playTree(GlobalParam::GetInstance()->_tree_time);
}

void ofApp::playTree(float tree_due){
	
	//if(_cur_tree_index>-1 && !_vec_tree[_cur_tree_index].display_finished) return;
	if(_is_playing_tree) return;

	_is_playing_tree=true;

	_is_asking_tree=false;
	_cur_tree_index=(_cur_tree_index+1)%_vec_tree.size();
	
	_vec_tree[_cur_tree_index].start(tree_due);
	
	ofLog()<<"Tree Started At "<<ofToString(GlobalParam::GetInstance()->getCalibTimeCode())<<" due= "<<ofToString(tree_due);
	

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key){
		case 'a':
			playTree();
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'c':
			_clock_num.toggleShow();
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

	_last_tree_index=_cur_tree_index;
	
	//playTree();
	ofLog()<<"Tree Finished At "<<ofToString(GlobalParam::GetInstance()->getCalibTimeCode());
	//goNextShow();

	tree_in_finished=false;
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
			break;
		case ASK_SCHEDULE:
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getTimetable";
			break;
		case ASK_OLD_TREE:
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getOldTree&mid="+ofToString(GlobalParam::GetInstance()->_machine_id);
			break;
		case ASK_NEW_TREE:
			f.action=GlobalParam::GetInstance()->_server_url+"?active=getNewTree&mid="+ofToString(GlobalParam::GetInstance()->_machine_id);
				//+"&DTime="+ofToString(_cur_show_starttime);
			//ofLog()<<"DTime= "<<ofToString(_cur_show_starttime);
			break;
	}

	httpUtils.addForm(f);
	
}
//void ofApp::httpResponse(HttpFormResponse &response){
void ofApp::httpResponse(ofxHttpResponse &response){
	
	//ofLog()<<ofToString(response.status) + ": " + (string)response.responseBody;
	
	if(response.status!=200){ // timeout fail
		if(!_init_request_finished){ // cannot get init-data
			
			sendSlackLog("[ERR] Cannot get init-data!");

			readSavedInitData();
			_init_request_finished=true;
			goNextShow();

		}else{ // cannot get new tree			
			
			sendSlackLog("[ERR] Cannot get new tree");
			playTree(_time_btw_show);
			
			httpUtils.clearQueue();
		}
		
		return;
	}




	Json::Value root;  
	Json::Reader reader;
	bool parse_success=reader.parse(response.responseBody,root);
	if(!parse_success){
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

		Json::Value result=root["table"];
		for(Json::ValueIterator itr=result.begin();itr!=result.end();itr++){			
			ShowType type= (ShowType)ofToInt((*itr)[Json::UInt(1)].asString());
			_map_schedule.insert(pair<int,ShowType>(ofToInt((*itr)[Json::UInt(0)].asString()),type));
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
			ATree t=ATree(atree["TreeCode"].asString(),atree["ToName"].asString(),atree["FromName"].asString(),atree["Message"].asString());
			insertNewTree(t);
			
			sendSlackLog("[Info] Get New Tree: (from "+atree["FromName"].asString()+", to "+atree["ToName"].asString());
		}
		/*_cur_tree_index=(_cur_tree_index+1)%_vec_tree.size();*/
		playTree(_time_btw_show);


	}else if(rtype.compare("getOldTree")==0){
		
		int result=root["result"].asInt();
		if(result==1){
			Json::Value atree=root["tree"];
			for(Json::ValueIterator itr=atree.begin();itr!=atree.end();itr++){				
				ATree t=ATree((*itr)["TreeCode"].asString(),(*itr)["ToName"].asString(),(*itr)["FromName"].asString(),(*itr)["Message"].asString());
				insertNewTree(t);
			}
			Json::Value val;
			val["tree"]=atree;
			GlobalParam::GetInstance()->writeJson(val,GlobalParam::OldTreeFile);

		}

		_init_request_finished=true;
		goNextShow();

	}



}

void ofApp::insertNewTree(ATree tree){

	if(_vec_tree.size()<GlobalParam::GetInstance()->_max_tree){
		_vec_tree.push_back(tree);
	}else{
		/*vector<ATree> tmp;
		tmp.push_back(tree);*/
		int s=_vec_tree.size();
		for(int i=0;i<s;++i){
			int p=(_cur_tree_index-i+s)%s;
			
			if(i==0) continue;
			
			if(i==s-1){
				_vec_tree[p]=tree;
				//ofLog()<<"Tree insert at "<<p;
			}else{
				_vec_tree[p]=_vec_tree[(p-1+s)%s];
				//ofLog()<<"Tree swap "<<p<<" <- "<<(p-1+s)%s;
			}

		}
		
	}

}

void ofApp::goNextShow(){

	ShowType next_show;
	float show_due;
	findNextShow(next_show,show_due);
	
	_time_btw_show=show_due;

	ofLog()<<"Next Show "<<ofToString(next_show);
	
	
	switch(next_show){
		case S_KV:
			_index_kv=(_index_kv+1)%4;
			ATreeImage::GetInstance()->_mov_kv[_index_kv].play();
			ATreeImage::GetInstance()->_mov_kv[_index_kv].setPosition(1-show_due/GlobalParam::GetInstance()->_kv_time);
			//if(show_due>=GlobalParam::GetInstance()->_kv_time)	ATreeImage::GetInstance()->_mov_kv[_index_kv].play();			
			//else ofLog()<<"Not enough time to play kv: "<<show_due;
			
			break;

		case S_BT:
			ATreeImage::GetInstance()->_mov_bt.play();
			ATreeImage::GetInstance()->_mov_bt.setPosition(1-show_due/GlobalParam::GetInstance()->_bt_time);
					
			//if(show_due>=GlobalParam::GetInstance()->_bt_time)	ATreeImage::GetInstance()->_mov_bt.play();
			//else ofLog()<<"Not enough time to play bt: "<<show_due;

			break;

		case S_TREE:
			/*_cur_tree_index=(_cur_tree_index+1)%_vec_tree.size();
			playTree(_time_btw_show);*/
					
			sendHttpRequest(ASK_NEW_TREE);
			_is_asking_tree=true;
			//sendThreadRequset(ASK_NEW_TREE);

			break;		
	}
	last_show=cur_show;
	cur_show=next_show;

	if(last_show!=cur_show){
		_anim_change_show.Restart();
		_last_tree_index=-1;
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
	
	auto itr=_map_schedule.find(now_code);
	if(itr==_map_schedule.end()){
		int a=4000;
		ShowType t;
		for(auto pair:_map_schedule){
			if(pair.first>now_code && now_code>a){
				due_time=(pair.first-now_code)*1000.0;					
				type=t;
				ofLog()<<"Find Nearest "<<ofToString(t)<<" at "<<a<<" to "<<pair.first<<" due= "<<due_time;
				_cur_show_starttime=pair.first;
				break;
			}
			a=pair.first;
			t=pair.second;
		}
	}else{
		type=_map_schedule[now_code];
		_cur_show_starttime=now_code;
		ofLog()<<"Find Show "<<ofToString(type)<<" at "<<now_code;
				
		switch(type){
			case S_KV:
				due_time=GlobalParam::GetInstance()->_kv_time;
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


void ofApp::readSavedInitData(){
	
	Json::Value timeset=GlobalParam::GetInstance()->parseFileToJson(GlobalParam::GetInstance()->TimeSetFile);

	GlobalParam::GetInstance()->_kv_time=ofToInt(timeset["KV_T"].asString())*1000;
	GlobalParam::GetInstance()->_tree_time=ofToInt(timeset["TREE_T"].asString())*1000;
	GlobalParam::GetInstance()->_bt_time=ofToInt(timeset["BT_T"].asString())*1000;
	GlobalParam::GetInstance()->_delta_second_to_server=ofToInt(timeset["S_T"].asString())-GlobalParam::GetInstance()->getTimeCode();


	Json::Value ftable=GlobalParam::GetInstance()->parseFileToJson(GlobalParam::GetInstance()->TimeTableFile);
	Json::Value table=ftable["table"];
	for(Json::ValueIterator itr=table.begin();itr!=table.end();itr++){			
		ShowType type= (ShowType)ofToInt((*itr)[Json::UInt(1)].asString());
		_map_schedule.insert(pair<int,ShowType>(ofToInt((*itr)[Json::UInt(0)].asString()),type));
	}

	Json::Value ftree=GlobalParam::GetInstance()->parseFileToJson(GlobalParam::GetInstance()->OldTreeFile);
	Json::Value atree=ftree["tree"];
	for(Json::ValueIterator itr=atree.begin();itr!=atree.end();itr++){				
		ATree t=ATree((*itr)["TreeCode"].asString(),(*itr)["ToName"].asString(),(*itr)["FromName"].asString(),(*itr)["Message"].asString());
		insertNewTree(t);
	}
}

void ofApp::sendSlackLog(string message){
	

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