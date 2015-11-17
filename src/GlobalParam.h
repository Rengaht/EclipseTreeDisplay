#ifndef GLOBAL_PARAM_H
#define GLOBAL_PARAM_H

#include "ofMain.h"
#include "json.h"

class GlobalParam{
	
	static GlobalParam* _instance;
	public:
		
		static string TimeSetFile,TimeTableFile,OldTreeFile;
		static string ParamFilePath;
		

		float _tree_time;
		float _kv_time;
		float _bt_time;
		
		float _tree_in_time;
		string _nothing;

		string _server_url;

		int _max_tree;
		
		int _machine_id;

		int _delta_second_to_server;


		GlobalParam(){}
		static GlobalParam* GetInstance(){
			if(!_instance)
				_instance=new GlobalParam();
			return _instance;
		}

		Json::Value parseFileToJson(string filename){
			Json::Value root;   // will contains the root value after parsing.
			Json::Reader reader;
			
			ofFile file=ofFile(filename);
			if(!file.exists()){
				ofLog()<<"No ParamFile exist! Create default one!";
				
				root=createDefaultJson();
			}else{
				ofBuffer buffer(file);
				string param_doc=buffer.getText();
			
				bool parse_success=reader.parse(param_doc,root);
				if(!parse_success){
					// report to the user the failure and their locations in the document.
					ofLog()<<"Failed to parse configuration\n"
						   <<reader.getFormatedErrorMessages();
					return root;
				}
			}
			return root;
		}

		void loadParameter(){
			
			Json::Value root=parseFileToJson(ParamFilePath);

			//_tree_time=root.get("TREE_TIME", 10000.0).asDouble();
			_tree_in_time=root.get("TREE_IN_TIME",3000.0).asDouble();
			_nothing=root.get("NOTHING","no pipi").asString();
			
			
			_server_url=root.get("SERVER","http://artgital.com").asString();

			_max_tree=root.get("MAX_TREE",10).asInt();

			_machine_id=root.get("MACHINE_ID",0).asInt();

		}
		Json::Value createDefaultJson(){
			
			Json::Value root;
			//root["TREE_TIME"]=10000.0;
			root["TREE_IN_TIME"]=3000.0;
			root["NOTHING"]="pipi";
			root["SERVER"]="http://artgital.com";
			root["MAX_TREE"]=10;
			root["MACHINE_ID"]=0;

			writeJson(root,ParamFilePath);

			return root;

		}
		void writeJson(Json::Value root,string file_name){
			Json::StyledWriter writer;
			string output=writer.write(root);

			ofstream out("data\\"+file_name);
			out<<output;
			out.close();

			
			ofLog()<<"Save Param File! ";

			
		}
		void saveJson(){

			Json::Value root;
			//root["TREE_TIME"]=_tree_time;
			root["TREE_IN_TIME"]=_tree_in_time;
			root["NOTHING"]=_nothing;
			root["SERVER"]=_server_url;
			root["MAX_TREE"]=_max_tree;
			root["MACHINE_ID"]=_machine_id;


			writeJson(root,ParamFilePath);

		}
		int getTimeCode(){
			return ofGetMinutes()*60+ofGetSeconds();
		}
		int getCalibTimeCode(){
			return getTimeCode()+_delta_second_to_server;
		}

};




#endif GLOBAL_PARAM_H