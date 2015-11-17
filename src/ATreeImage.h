#ifndef A_TREE_IMAGE_H
#define A_TREE_IMAGE_H
#include "ofMain.h"
#include "ofxHapPlayer.h"

class ATreeImage{
	static ATreeImage* _instance;
public:
	ofImage* _image_tree;
	ofImage* _image_topdeco;
	ofImage* _image_deco;
	
	ofImage _image_ref;

	ofImage _image_number[11];


	ofColor* _tree_color;
	ofxTrueTypeFontUC _font_LBB_title;
	ofxTrueTypeFontUC _font_LBB_name;

	ofRectangle _font_rect;

	ofVideoPlayer _mov_tree_bg;
	ofVideoPlayer _mov_kv[4];
	ofVideoPlayer _mov_bt;


	ATreeImage(){}

	static ATreeImage* GetInstance(){
		if(!_instance)
			_instance=new ATreeImage();
		return _instance;
	}

	void loadImage(){

		_image_ref.loadImage("image/ref.jpg");

		_image_tree=new ofImage[4];
		for(int i=0;i<4;++i){
			_image_tree[i].loadImage("image/tree_"+ofToString(i+1)+".png");
			_image_tree[i].setAnchorPercent(.5,.5);
		}
		_image_topdeco=new ofImage[4];
		for(int i=0;i<4;++i){
			_image_topdeco[i].loadImage("image/topdeco_"+ofToString(i+1)+".png");
			_image_topdeco[i].setAnchorPercent(.5,.5);		
		}
		_image_deco=new ofImage[5*2];
		for(int i=0;i<5;++i){
			_image_deco[i*2].loadImage("image/deco_"+ofToString(i+1)+"_1.png");
			_image_deco[i*2].setAnchorPercent(.5,.5);

			_image_deco[i*2+1].loadImage("image/deco_"+ofToString(i+1)+"_2.png");
			_image_deco[i*2+1].setAnchorPercent(.5,.5);
		}

		_tree_color=new ofColor[4];
		_tree_color[0]=ofColor(15,117,188);
		_tree_color[2]=ofColor(212,113,104);
		_tree_color[1]=ofColor(0,61,126);
		_tree_color[3]=ofColor(130,0,52);

		_font_LBB_title.loadFont("font/Li_Bold_Black.ttc",64,true,true);
		_font_rect=_font_LBB_title.getStringBoundingBox(ws2s(L"§Ú³Ì°Q¹½§A©Ô©Ô©Ô©Ô©Ô"),0,0);
		
		_font_LBB_name.loadFont("font/Li_Bold_Black.ttc",55,true,true);
		

		_mov_tree_bg.setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer));
		_mov_tree_bg.setPixelFormat(ofPixelFormat::OF_PIXELS_RGBA);
		_mov_tree_bg.loadMovie("movie/tree_bg.mov");
		_mov_tree_bg.setLoopState(ofLoopType::OF_LOOP_NORMAL);

		
		for(int i=0;i<4;++i){
			_mov_kv[i].setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer));
			_mov_kv[i].setPixelFormat(ofPixelFormat::OF_PIXELS_RGB);
			_mov_kv[i].loadMovie("movie/KV00"+ofToString(i+1)+".mov");
			_mov_kv[i].setLoopState(ofLoopType::OF_LOOP_NONE);
			//ofLog()<<"kv movie duration"<<_mov_kv.getDuration();
		}

		_mov_bt.setPlayer(ofPtr<ofxHapPlayer>(new ofxHapPlayer));
		_mov_bt.setPixelFormat(ofPixelFormat::OF_PIXELS_RGB);
		_mov_bt.loadMovie("movie/btbt.mov");
		_mov_bt.setLoopState(ofLoopType::OF_LOOP_NONE);
		//ofLog()<<"bt movie duration"<<_mov_bt.getDuration();


		for(int i=0;i<11;++i){
			_image_number[i].loadImage("image/NUMBER/"+ofToString(i)+".png");
			_image_number[i].setAnchorPercent(.5,.5);
		}


	}
	static string ws2s(const wstring& wstr){
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, 0);
		string strTo( size_needed, 0 );
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, 0);
  
		return strTo;
	}

};


#endif