#include "ofMain.h"
#include "ofApp.h"

//========================================================================

//string GlobalParam::ParamFilePath="paramparam.json";
string GlobalParam::TimeSetFile="timetime.json";
string GlobalParam::TimeTableFile="tabletable.json";
string GlobalParam::OldTreeFile="oldoldtree.json";
string GlobalParam::ParamFilePath="paramparam.json";

GlobalParam *GlobalParam::_instance=0;
ATreeImage *ATreeImage::_instance=0;

int main( ){

	ofSetupOpenGL(1080,1920,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:



	ofRunApp(new ofApp());

}
