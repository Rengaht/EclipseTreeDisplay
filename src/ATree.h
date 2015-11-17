#ifndef A_TREE_H
#define A_TREE_H

#include "ofxTrueTypeFontUC.h"
#include "ATreeImage.h"
#include "GlobalParam.h"

#include "FrameAnimation.h"
#include "FlyInAnimation.h"



class ATree{

	private:
		int* _arr_part_index;
		string* _arr_word;

		//FrameAnimation _animation_tree_in;
		FlyInAnimation* _fly_tree_in;
		FlyInAnimation* _fly_topdeco_in;

		FrameAnimation* _arr_animation_deco;
		FlyInAnimation* _arr_fly_deco_in;
		
		FlyInAnimation* _arr_fly_text_in;
		
		
		FrameAnimation* _anim_back_in;
		FrameAnimation* _anim_back_video_in;



		float _vel_topdeco,_acc_topdeco;
		ofVec2f _pos_topdeco;

		float _display_time;

		void setup(int index_tree,int index_top,int index_deco1,int index_deco2,int index_deco3,string from_name,string to_name,string word);
		
		bool display_finished;
		bool anim_in_finished;

		


	public:

		static const ofVec2f DecoPos[12];
		static const ofVec2f TextPos[3];

		//ATreeImage *TreeImage;
		ATree();
		ATree(int index_tree,int index_top,int index_deco1,int index_deco2,int index_deco3,string from_name,string to_name,string word);
		ATree(string tree_code,string from_name,string to_name,string word);

		void update(float dt);
		void start(float set_due);

		void draw();
		int getBackIndex();

		
		static ofEvent<int> event_finish;
		static ofEvent<int> event_in_finish;
		
};






#endif