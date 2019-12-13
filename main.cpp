#include <iostream>
#include <fstream>
#include <sys/time.h>
#include <cstddef>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


#define BUFFER_SIZE 2000

int b,flg=0;
int ct;
int ins=0;

//Including the data structure files in the main file
ofstream fout;
#include "heap.h"
#include "rbt.h"
#include "heap.cpp"
#include "rbt.cpp"




class construct{
private:
	int counter;
	rbt* myrbt;
	heap* myheap;
protected:

public:
	void print(int buildingNum); //Print(buildingNumber,executed_time,total_time) for given buildingNum
	void print(int buildingNum1,int buildingNum2); //Print(buildingNum,executed_time,total_time) for given range of buildingNums
	void insert(int buildingNum, int time); //Insert buldingNum and and total time
	construct();
	void syncTime(int time); //Synchronizes time between global time and counter
	int dispatch(int); //Send building for construction
	int ifbuild(); //Checks if any building is left to build

};

int construct::dispatch(int x){
	int retval = 0;

	heapnode* thisnode = &(myheap->root[1]);

	thisnode->exec_time+=x;

	if(thisnode->total_time-thisnode->exec_time<=0){
		retval = thisnode->exec_time-thisnode->total_time;

        b=thisnode->bnum;
        flg=1;
		myrbt->deletenode(thisnode->twin);
		myheap->removeMin();

	}
	myheap->heapify();

	return retval;
}
void construct::syncTime(int time){
	int param,remain;
	while(time>counter){
		if(time-counter < 5 && ins==0)
			{
			    param = time-counter;
	    }
		else{
            param = 5;

		}


		if(ifbuild()){
			remain = dispatch(param);
			counter+=param-remain; ct=counter;
		}
		else{
            counter=time;
			ct=counter;

		}

	}
}
construct::construct(){
		counter=0;
		myheap = new heap(BUFFER_SIZE);
		myrbt = new rbt;

	}
void construct::print(int buildingNum){

	myrbt->findnode(buildingNum);
}

void construct::print(int buildingNum1,int buildingNum2){
	myrbt->inorder(buildingNum1,buildingNum2);

}

int construct::ifbuild(){
	return myheap->last;
}


void construct::insert(int buildingNum,int totaltime){

	rbtnode* temp2 = myrbt->insert(buildingNum);

	//insert into heap
	myheap->insert(buildingNum,0,totaltime,temp2);

}



int main(int argc, char** argv){

	construct myconstruct; // create a construct object

	char *ptr,*ptr1,buff[100];
	int num;
	ifstream file;


	if(argc<2){
		cout<<"Error: Not enough arguments"<<endl;
		exit(1);
	}

	file.open(argv[1]); // open input file

	fout.open("output_file.txt"); //open output file



	for( std::string line; getline( file, line, '\n'); ){


		//variables for string parsing
		int temp1,temp2;
		ptr = strcpy(buff,line.c_str());
		ptr = strtok(ptr,":");
		num = atoi(ptr);

		ptr = strtok(NULL,"(");
        if(strcmp(ptr+1,"Insert")==0){
         ins=1;
        }
        else{
            ins=0;
        }

        if(strcmp(ptr+1,"PrintBuilding")==0){
        	ptr = strtok(NULL,")");
        	int i,flag=0;
        	for(i=0;i<strlen(ptr);i++){
        		if(ptr[i]==',') {
        			flag=1;
        			break;
        		}
        	}

   	     	if(flag==1){
   	     		// print range of buildings
   	     		ptr1 = strtok(ptr,",");
   	     		ptr += strlen(ptr1)+1;
				myconstruct.print(atoi(ptr1),atoi(ptr));

        	}
        	else{
        		// print single job
   	    		myconstruct.print(atoi(ptr));
        	}


         }

		//schedule buildings until counter = command time
		myconstruct.syncTime(num);



        // Insert new building
        if(strcmp(ptr+1,"Insert")==0){
        	ptr = strtok(NULL,",");
        	temp1 = atoi(ptr);
        	ptr = strtok(NULL,")");
        	temp2 = atoi(ptr);
        	myconstruct.insert(temp1,temp2);
        }




    }
int r;
    // End of input file. schedule all the pending buildings
    while(myconstruct.ifbuild()){
    	r=myconstruct.dispatch(5);
    	ct+=5-r;
    	if(flg==1){
            flg=0;
            fout<<"("<<b<<","<<ct<<")"<<endl;
    	}
    }


    //close file handlers
	file.close();
	fout.close();

	return 0;

}

