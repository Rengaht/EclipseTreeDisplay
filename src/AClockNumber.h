#ifndef ACLOCK_NUMBER_H
#define ACLOCK_NUMBER_H


class AClockNumber{
	
	public:
		
		AClockNumber(){
			_anim_show=FrameAnimation(500);
			_is_show=false;
			_anim_show.Restart();
		}

		void upate(float delta){
			_anim_show.Update(delta);
		}

		void draw(){
			
			ofPushStyle();
			if(_is_show) ofSetColor(255,255*_anim_show.GetPortion());
			else ofSetColor(255,255*(1-_anim_show.GetPortion()));

			float _swid=ATreeImage::GetInstance()->_image_number[0].width;
			float _cwid=.5*_swid;

			ofPushMatrix();
			ofTranslate(154.5,1698.5);
				
			drawTwoNumber(ofGetHours(),_swid);
			ofTranslate(_swid*1.5+_cwid*.5,0);
			ATreeImage::GetInstance()->_image_number[10].draw(0,20);

			ofTranslate(_swid*.5+_cwid*.5,0);
			
			int cal_time=GlobalParam::GetInstance()->getCalibTimeCode();
			drawTwoNumber(floor(cal_time/60.0),_swid);


			ofTranslate(_swid*1.5+_cwid*.5,0);
			ATreeImage::GetInstance()->_image_number[10].draw(0,20);

			ofTranslate(_swid*.5+_cwid*.5,0);
			drawTwoNumber(cal_time%60,_swid);
			

			ofPopMatrix();

			ofPopStyle();
		}
		void toggleShow(){
			_anim_show.Restart();
			_is_show=!_is_show;
		}
		void setVisible(bool visible){
			if(visible && !_is_show){
				ofLog()<<"Show Clock!!";
				_anim_show.Restart();
				_is_show=true;
				return;
			}
			if(!visible && _is_show){
				ofLog()<<"Hide Clock!!";
				_anim_show.Restart();
				_is_show=false;
				return;			
			}
			
		}
		

	private:

		FrameAnimation _anim_show;
		bool _is_show;

		int* getNumber(int num){
			int* a=new int[2];
			if(num<10){
				a[0]=0;
				a[1]=num;
			}else{
				a[0]=floor(num/10);
				a[1]=num%10;
			}
			return a;
		}
		void drawTwoNumber(int num,float _swid){
			int* n=getNumber(num);
			for(int i=0;i<2;++i){
				ATreeImage::GetInstance()->_image_number[n[i]].draw(_swid*i,0);
			}
		}


};



#endif