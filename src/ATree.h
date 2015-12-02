#ifndef A_TREE_H
#define A_TREE_H

#include "ofxTrueTypeFontUC.h"
#include "GlobalParam.h"
#include "ATreeImage.h"


#include "FrameAnimation.h"
#include "FlyInAnimation.h"



class ATree{

	private:
		

		//FrameAnimation _animation_tree_in;
		FlyInAnimation _fly_tree_in;
		FlyInAnimation _fly_topdeco_in;

		FrameAnimation _arr_animation_deco[12];
		FlyInAnimation _arr_fly_deco_in[12];
		
		FlyInAnimation _arr_fly_text_in[3];
		
		FrameAnimation _anim_tid_in;
		
		FrameAnimation _anim_back_in;
		FrameAnimation _anim_back_video_in;



		float _vel_topdeco,_acc_topdeco;
		ofVec2f _pos_topdeco;

		float _display_time;

		void setup(int index_tree,int index_top,int index_deco1,int index_deco2,int index_deco3,string from_name,string to_name,string word,string id);
		
		bool display_finished;
		bool anim_in_finished;

		bool _is_started;

		int _start_time;

	public:
		int _arr_part_index[5];
		string _arr_word[4];

		static const ofVec2f DecoPos[12];
		static const ofVec2f TextPos[3];

		//ATreeImage *TreeImage;
		ATree();
		ATree(int index_tree,int index_top,int index_deco1,int index_deco2,int index_deco3,string from_name,string to_name,string word,string id);
		ATree(string tree_code,string from_name,string to_name,string word,string id);

		void update(float dt);
		void start(float set_due);
		void reset();

		void draw();
		int getBackIndex();

		
		static ofEvent<int> event_finish;
		static ofEvent<int> event_in_finish;
		
};






#endif