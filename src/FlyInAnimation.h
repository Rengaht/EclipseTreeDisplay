#ifndef FLYIN_ANIMATION_H
#define FLYIN_ANIMATION_H

class FlyInAnimation{

private:
	FrameAnimation _anim_fly,_anim_vib;
	ofVec2f _pos_start,_pos_end;

	float _vel,_acc;

public:
	FlyInAnimation(){
	
	}
	FlyInAnimation(float fly_due,float fly_delay,ofVec2f set_start,ofVec2f set_end){
		_anim_fly=FrameAnimation(fly_due,fly_delay);
		_anim_vib=FrameAnimation(fly_due*.8,fly_due*.9+fly_delay);

		_pos_start=set_start;
		_pos_end=set_end;

		_vel=(_pos_end.x-_pos_start.x)/fly_due;
		_acc=(_pos_end.y-_pos_start.y)*2/pow(fly_due,2);

	}
	void start(){
		_anim_fly.Restart();
		_anim_vib.Restart();
	}
	void update(float dt){
		_anim_fly.Update(dt);
		_anim_vib.Update(dt);
	}

	ofVec2f getFlyPos(){
		float t=_anim_fly.GetPos();
		
		float x=_pos_start.x+_vel*t;
		float y=_pos_start.y+.5*_acc*pow(t,2);

		return ofVec2f(x,y);
	}
	float getVib(){
		return (_anim_vib.ani_t>0)?(1.0-_anim_vib.GetPortion()):0;
	}

	float getAngle(){
		/*ofVec2f pos=getFlyPos();
		return atan2(pos.y-_pos_end.y,pos.x-_pos_end.x);*/
		float t=_anim_fly.GetPortion();
		return (t==1)?0:atan2(_acc*_anim_fly.GetPos(),_vel)+PI/2;
	}

	bool isFinished(){
		return _anim_vib.isFinished();
	}


};




#endif 