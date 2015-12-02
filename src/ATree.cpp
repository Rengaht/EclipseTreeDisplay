
#include "ATree.h"

//ATreeImage* GTREE_IMAGE=nullptr;
//GlobalParam* GPARAM=nullptr;

const ofVec2f ATree::DecoPos[12]={ofVec2f(.04,-.11),ofVec2f(.0,.12),ofVec2f(-.23,.25),ofVec2f(.1,.25),
								  ofVec2f(-.05,-.06),ofVec2f(.12,.07),ofVec2f(-.15,.15),ofVec2f(.25,.22),
								  ofVec2f(.05,0),ofVec2f(-.1,.05),ofVec2f(.12,.17),ofVec2f(-.05,.22)};

const ofVec2f ATree::TextPos[3]={ofVec2f(0,.19),ofVec2f(0,.135),ofVec2f(0,.08)};

ofEvent<int> ATree::event_finish=ofEvent<int>();
ofEvent<int> ATree::event_in_finish=ofEvent<int>();

ATree::ATree(){
	int deco[3]={-1,-1,-1};
	for(int i=0;i<3;++i){
		deco[i]=(int)ofRandom(5);				
		while(deco[i]==deco[(i+1)%3] || deco[i]==deco[(i+2)%3]){
			deco[i]=(int)ofRandom(5);
		}
	}
	string a=ATreeImage::GetInstance()->ws2s(L"長頸鹿");
	setup(ofRandom(4),ofRandom(4),deco[0],deco[1],deco[2],
		ATreeImage::GetInstance()->ws2s(L"長頸鹿"),ATreeImage::GetInstance()->ws2s(L"皮皮"),ATreeImage::GetInstance()->ws2s(L"聖誕超快樂!!!"),ofToString(0));
		//"test","test","test",0);
		
}

ATree::ATree(int index_tree,int index_top,int index_deco1,int index_deco2,int index_deco3,string from_name,string to_name,string word,string id){
	setup(index_tree,index_top,index_deco1,index_deco2,index_deco3,from_name,to_name,word,id);
	
					
}
ATree::ATree(string tree_code,string from_name,string to_name,string word,string id){
	
	setup(tree_code[0]-'0',tree_code[1]-'0',tree_code[2]-'0',tree_code[3]-'0',tree_code[4]-'0',from_name,to_name,word,id);
	//setup(tree_code[0]-'0',tree_code[1]-'0',5,6,7,from_name,to_name,word);
}




void ATree::setup(int index_tree,int index_top,int index_deco1,int index_deco2,int index_deco3,string from_name,string to_name,string word,string id){
			
	ofLog()<<ofGetTimestampString()<<"New Tree: "<<index_tree<<index_top<<index_deco1<<index_deco2<<index_deco3<<from_name<<" to "<<to_name<<" : "<<word;

	//_arr_part_index=new int[5];
	_arr_part_index[0]=index_tree;
	_arr_part_index[1]=index_top;
	_arr_part_index[2]=ofClamp(index_deco1,0,GlobalParam::GetInstance()->_deco_count-1);
	_arr_part_index[3]=ofClamp(index_deco2,0,GlobalParam::GetInstance()->_deco_count-1);
	_arr_part_index[4]=ofClamp(index_deco3,0,GlobalParam::GetInstance()->_deco_count-1);

	//_arr_word=new string[4];
	/*_arr_word[0]="From "+from_name;
	_arr_word[1]=word;
	_arr_word[2]="To "+to_name;*/
	_arr_word[0]=from_name;
	_arr_word[1]=word;
	_arr_word[2]=to_name;
	_arr_word[3]=id;//ATreeImage::GetInstance()->ws2s(L"第")+id+ATreeImage::GetInstance()->ws2s(L"棵聖誕樹");
	
	float total_time=GlobalParam::GetInstance()->_tree_in_time;
	float stime=total_time/6.0;
	//float back_time=stime*2;

	_anim_back_in=FrameAnimation(stime*1.5);
	_anim_back_video_in=FrameAnimation(stime*.2,stime*1.2);

	//_anim_back_in->setElastic(true);

	//btime+=200*pace;
	_fly_tree_in=FlyInAnimation(stime*.5,stime*2,ofVec2f(ofGetWidth()*ofRandom(0,2),-ofGetHeight()*1.4),ofVec2f(0,0));

	
	//_arr_fly_deco_in=new FlyInAnimation[12];
	
	float decow=ofGetWidth();
	float decoh=ofGetHeight();

	for(int i=0;i<12;++i){
		ofVec2f pos=DecoPos[i];
		pos.x*=decow;
		pos.y*=decoh*1.15;
		_arr_fly_deco_in[i]=FlyInAnimation(ofRandom(.5,.7)*stime,ofRandom(.2,.5)*stime+stime*2.5,ofVec2f(ofGetWidth()*ofRandom(-1,1),-ofGetHeight()*ofRandom(2,3)),pos);
	}
			
	_fly_topdeco_in=FlyInAnimation(stime*.4,stime*3.7,ofVec2f(ofGetWidth()*ofRandom(-.5,.5),-ofGetHeight()*1.2),ofVec2f(0,0));
	
	//_arr_fly_text_in=new FlyInAnimation[3];
	for(int i=0;i<3;++i){
		ofVec2f pos=TextPos[i];
		pos.x*=ofGetWidth();
		pos.y*=ofGetHeight();
		_arr_fly_text_in[i]=FlyInAnimation(stime*.5,stime*4.4+stime*.5*(i),ofVec2f(pos.x+ofGetWidth()*ofRandom(-.5,.5),-ofGetHeight()*ofRandom(2,3)),pos);
	}

	


	//_arr_animation_deco=new FrameAnimation[12];
	for(int i=0;i<12;++i){
		_arr_animation_deco[i]=FrameAnimation(500);
		_arr_animation_deco[i].setLoop(true);
		_arr_animation_deco[i].Start(ofRandom(1));
	}
		
	_anim_tid_in=FrameAnimation(stime*.5,stime*6);
	_anim_tid_in.setElastic(true);
	
	reset();

}

void ATree::update(float dt){

	//if(!_is_started) return;

	_anim_back_in.Update(dt);
	_anim_back_video_in.Update(dt);
	
	_fly_tree_in.update(dt);	
	_fly_topdeco_in.update(dt);

	_anim_tid_in.Update(dt);
	
	for(int i=0;i<12;++i) _arr_animation_deco[i].Update(dt);
	for(int i=0;i<12;++i) _arr_fly_deco_in[i].update(dt);				
	for(int i=0;i<3;++i) _arr_fly_text_in[i].update(dt);				

	if(!anim_in_finished && _anim_back_in.isFinished()){
		int a=1;
		ofNotifyEvent(event_in_finish,a);
		anim_in_finished=true;
	}

	_display_time-=dt;
	//cout<<_display_time<<endl;

	if(_is_started && !display_finished && _display_time<=0){
		int now=GlobalParam::GetInstance()->getCalibTimeCode();
		if(_start_time==now){
			cout<<_display_time;
			return;
		}
		int a=_start_time;
		ofNotifyEvent(event_finish,a);
		display_finished=true;		
	}

}
		
void ATree::start(float set_due){

	//play_due=set_due;

	_anim_back_in.Restart();
	_anim_back_video_in.Restart();
	_anim_tid_in.Restart();

	_fly_tree_in.start();
	
	_fly_topdeco_in.start();

	
	
	for(int i=0;i<12;++i) _arr_animation_deco[i].Restart(ofRandom(1));		
	for(int i=0;i<12;++i) _arr_fly_deco_in[i].start();		
	for(int i=0;i<3;++i) _arr_fly_text_in[i].start();		

	_display_time=set_due;
	if(_display_time<=0){
		_display_time=GlobalParam::GetInstance()->_tree_time;
	}


	display_finished=false;
	anim_in_finished=false;
	
	_is_started=true;

	_start_time=GlobalParam::GetInstance()->getCalibTimeCode();

}

void ATree::reset(){
	
	_is_started=false;
	//_display_time=GlobalParam::GetInstance()->_tree_time;
}

void ATree::draw(){

	//if(display_finished) return;


	/*ofSetColor(TreeImage->_tree_color[_arr_part_index[1]]);
	ofFill();
	ofRect(0,0,ofGetWidth(),ofGetHeight());*/
		
	//ofBackground(ATreeImage::GetInstance()->_tree_color[_vec_tree[_cur_tree_index].getBackIndex()]);	
	
	float back_pos=_anim_back_in.GetPortion();
	ofPushMatrix();

	ofTranslate(0,-(float)ofGetHeight()*(1-back_pos));
	//ofRotate(_fly_back_in->getAngle());
		ofPushStyle();
		ofSetColor(ATreeImage::GetInstance()->_tree_color[_arr_part_index[1]]);
		ofRect(0,0,ofGetWidth(),ofGetHeight());
		ofPopStyle();
		
		ofPushStyle();
		ofSetColor(255,_anim_back_video_in.GetPortion()*255.0);
		ATreeImage::GetInstance()->_mov_tree_bg.draw(0,0);
		ofPopStyle();
	ofPopMatrix();
	//ATreeImage::GetInstance()->_image_ref.draw(0,0);

	
	if(back_pos<1){
		return;
	}


	ofPushMatrix();
	ofTranslate(ofGetWidth()/2,ofGetHeight()*.56);
	//ofScale(.5445,.5445);

	
	
	ofPushMatrix();
	ofVec2f tree_pos=_fly_tree_in.getFlyPos();
	
	float vib=_fly_tree_in.getVib()+_fly_topdeco_in.getVib();
	
	ofTranslate(tree_pos.x,tree_pos.y);
	ofRotate(_fly_tree_in.getAngle());

	ofTranslate(vib*ofRandom(-5,5),vib*ofRandom(-5,5));
	ofRotate(ofRandom(-1,1)*PI/3.0*vib);			

		ATreeImage::GetInstance()->_image_tree[_arr_part_index[0]].draw(0,0);

		for(int i=0;i<12;++i){

			int ifr=_arr_animation_deco[i].loop_count%2;
			int index=_arr_part_index[2+(int)floor(i/4)];

			ofVec2f pos=_arr_fly_deco_in[i].getFlyPos();
			float vib=_arr_fly_deco_in[i].getVib();

			ofPushMatrix();
			ofTranslate(pos.x,pos.y);
			ofRotate(_arr_fly_deco_in[i].getAngle());

			ofTranslate(vib*ofRandom(-10,10),vib*ofRandom(-10,10));
			ofRotate(ofRandom(-1,1)*PI/2.0*vib);			
			ofScale(.95,.95);
				ATreeImage::GetInstance()->_image_deco[index*2+ifr].draw(0,0);

			ofPopMatrix();
		}

	ofPopMatrix();



	ofPushMatrix();
	//ofLog()<<tvib;
	/*float ttop=_animation_topdeco_in.GetPos();
	float tvib=1.0-_animation_topdeco_vib.GetPortion();*/
	ofVec2f topdeco_pos=_fly_topdeco_in.getFlyPos();
	float tvib=_fly_topdeco_in.getVib();

	ofTranslate(topdeco_pos.x,topdeco_pos.y);
	ofRotate(_fly_topdeco_in.getAngle());

	ofTranslate(tvib*ofRandom(-10,10),tvib*ofRandom(-10,10));
	ofRotate(ofRandom(-1,1)*PI/2.0*tvib);			
		ATreeImage::GetInstance()->_image_topdeco[_arr_part_index[1]].draw(0,0);

	ofPopMatrix();
	
	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofGetWidth()/2,0);
	
	for(int i=0;i<3;++i){

		if(_arr_word[i].empty()) return;


		ofVec2f pos=_arr_fly_text_in[i].getFlyPos();
		float vib=_arr_fly_text_in[i].getVib();

		ofPushMatrix();
		ofTranslate(pos.x,pos.y);
		ofRotate(_arr_fly_deco_in[i].getAngle());

		ofTranslate(vib*ofRandom(-5,5),vib*ofRandom(-5,5));
		ofRotate(ofRandom(-1,1)*PI/2.0*vib);			

		ofRectangle rect;
		if(i==1) rect=ATreeImage::GetInstance()->_font_LBB_title.getStringBoundingBox(_arr_word[i],0,0);
		else if(i==2) rect=ATreeImage::GetInstance()->_font_LBB_name.getStringBoundingBox("To "+_arr_word[i],0,0);
		else rect=ATreeImage::GetInstance()->_font_LBB_name.getStringBoundingBox("From "+_arr_word[i],0,0);

		float scale_word=.8;
		if(i==1){			
			ATreeImage::GetInstance()->_font_LBB_title.drawString(_arr_word[i],-rect.width/2,rect.height/2);				
		}else if(i==2){
			ATreeImage::GetInstance()->_font_LBB_name.drawString("To "+_arr_word[i],-ATreeImage::GetInstance()->_font_rect.width/2.0,rect.height/2);	
		}else{						
			ATreeImage::GetInstance()->_font_LBB_name.drawString("From "+_arr_word[i],ATreeImage::GetInstance()->_font_rect.width/2.0-rect.width,rect.height/2);	
		}
		ofPopMatrix();
	}

	ofPopMatrix();

	float cs_=.9;
	ofPushStyle();
	//ofSetColor(255,255*_anim_tid_in->GetPortion());
	string str_index=ATreeImage::GetInstance()->ws2s(L"第")+_arr_word[3]+ATreeImage::GetInstance()->ws2s(L"棵聖誕樹");
	ofRectangle rect=ATreeImage::GetInstance()->_font_LBB_name.getStringBoundingBox(str_index,0,0);
	ofPushMatrix();
	float y_=ofLerp(1.2,0.953,_anim_tid_in.GetPortion());
		ofTranslate(ofGetWidth()/2,ofGetHeight()*y_);	
		ofScale(cs_,cs_);
		ATreeImage::GetInstance()->_font_LBB_name.drawString(str_index,-rect.width/2.0*cs_,0);		
	ofPopMatrix();
	
	ofPopStyle();

}


int ATree::getBackIndex(){
	return _arr_part_index[1];
}


