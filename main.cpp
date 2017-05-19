#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<sstream>
#include<stdlib.h>


#include "systemc.h"

//#include "producer.cpp"
//#include "consumer.cpp"


using namespace std;

struct IN{
	int b;
	int d;
	int c;
};

struct OUT{
	int b;
	int d;
	int p;
};

struct LINK{
	int b;
	int d;
	int p;
};

struct IN_r{
	sc_signal<int> b;
	sc_signal<int> d;
	sc_signal<int> c;
};


struct OUT_r{
	sc_signal<int> b;
	sc_signal<int> d;
	sc_signal<int> p;
};



class PE
{
	int pe_config;
	int pe_config_d;
	int pe_config_pb;
	IN_r in_r_pe;
	OUT_r out_r_pe;

	int index;
	

	public:	
		void getconfig(int index);
		void displayconfig(void);
		void writeconfig(void);
		OUT process(IN);
};


void PE :: getconfig(int pe_num)
{
	string line;

	ifstream config;
	config.open("fir_config.dat");
	if(config.fail()){
		cout<<"could not open the configuration file"<<endl;
		exit(1);
	}
	index=pe_num;
	//cout<<"   index   ---  "<<index<<endl;	

	for(int i=0;i<index*3;i++){
		getline(config,line);
	}
	
	getline(config,line);
	stringstream ss;
	ss<<hex<<line;
	ss>>pe_config;
	
	getline(config, line);
	ss.clear();
	ss<<hex<<line;
	ss>>pe_config_d;
	
	getline(config, line);
	ss.clear();
	ss<<hex<<line;
	ss>>pe_config_pb;
	
	config.close();	
}

void PE :: displayconfig(void)
{
	cout<<endl;
	cout<<"DSP config: ";
	cout<<setfill('0')<<setw(8)<<hex<<pe_config<<endl;
	cout<<"Immediate d: ";
	cout<<setfill('0')<<setw(8)<<hex<<pe_config_d<<endl;
	cout<<"Immediate pb: ";
	cout<<setfill('0')<<setw(8)<<hex<<pe_config_pb<<endl;
}

void PE :: writeconfig(void)
{
	ofstream config_out;
	config_out.open("config_out.dat");
	config_out<<setfill('0')<<setw(8)<<hex<<pe_config;
	config_out<<endl;
	config_out<<setfill('0')<<setw(8)<<hex<<pe_config_d;
	config_out<<endl;
	config_out<<setfill('0')<<setw(8)<<hex<<pe_config_pb;
	config_out<<endl;
	config_out.close();

}

//IN_r in_r_pe;
//OUT_r out_r_pe;


OUT PE :: process(IN in){
	OUT temp,out;	

	ostringstream filename;
	filename<<"Simulation_Results/PE_"<<index<<".txt";

	ofstream results;
	results.open((filename.str().c_str()), ios::app);
	if(results.fail()){
		cout<<"could not open the results file"<<endl;
		exit(1);
	}

	
	
	sc_time t(1,SC_MS);
	t=sc_time_stamp();
	//t.to_seconds()*100)+1;
	results<<"clock "<<(t.to_seconds()*100)+1<<" -----";
		
	
	in_r_pe.b.write(in.b);
	in_r_pe.c.write(in.c);
	in_r_pe.d.write(in.d);

	results<<"IN "<<in_r_pe.b.read()<<" "<<in_r_pe.d.read()<<" "<<in_r_pe.c.read()<<" -----";

	temp.p = (in_r_pe.b.read())*pe_config_d + in_r_pe.c.read();
	temp.b = in_r_pe.b.read();
	temp.d = in_r_pe.d.read();

	out_r_pe.p.write(temp.p);
	out_r_pe.b.write(temp.b);
	out_r_pe.d.write(temp.d);

	results<<"OUT "<<out_r_pe.b.read()<<" "<<out_r_pe.d.read()<<" "<<out_r_pe.p.read()<<" -----"<<endl;

	out.p=out_r_pe.p.read();
	out.b=out_r_pe.b.read();
	out.d=out_r_pe.d.read();
	results.close();
	
	return out;
}


PE pe[4];
IN in[3];		
OUT out[3];
LINK linker[3];

IN_r in_r[3];
OUT_r out_r[3];
/*
 * This module generates data on every clock cycle,
 * with an incremental basis, for all of its lifetime.
 */
class Producer : public sc_module {
    private:
        int cnt;
	
    public:
        sc_out<int> value;  // data port
        sc_in<bool> clk;    // clock port

        SC_CTOR(Producer) {
            cnt = 0;
            SC_THREAD(produce);
            // make the "produce" thread sensitive to the positive clock level
            sensitive << clk.pos();
        }

        void produce() {
            while (true) {  // do forever:
                wait();     // wait the beginning of a clock cycle
                // possibly update the counter
                if (++cnt == 1000)
                    cnt = 0;
                // write data
                value.write(cnt);
            }
        }
};

/*
* Consumer waits for data appearing on its bus, and
 * echoes such data.
 */

int valid=0;

class Consumer : public sc_module {
    public:
        sc_in<int> value;       // data port
	sc_in<bool> clk;    // clock port
	sc_signal<int> reg;
	

        SC_CTOR(Consumer) {
            SC_THREAD(consume);
            // make "consume()" sensible to changes on the value port
            sensitive << value;

            SC_THREAD(process_pe);
            // make "consume()" sensible to changes on the value port
            sensitive << clk.pos();

            //SC_THREAD(process_pe1);
            // make "consume()" sensible to changes on the value port
            //sensitive << clk.pos();

            //SC_THREAD(process_pe2);
            // make "consume()" sensible to changes on the value port
            //sensitive << clk.pos();

        }

        // module activity function
        void consume() {
            int x;       	

            while (true) {
                // wait for new data to appear on the value port
		//valid=0;
                wait();
                // fetch it and display its val
                x = value.read();
		reg.write(x);
		in[0].b=x;
		in[0].c=0;
		in[0].d=0;
		valid = 1;
                cout << x<<" Reg = "<<reg.read()<<endl;
		//cout << " OUT0="<<out[0].p<<" OUT1="<<out[1].p<<endl;
            }
        }

	void process_pe() {
            int x;       	

            while (true) {
                // wait for new data to appear on the value port
		wait();
                // fetch it and display its val
		out[0] = pe[0].process(in[0]);
		
		out_r[0].b.write(out[0].b);
		out_r[0].p.write(out[0].p);
		out_r[0].d.write(0);		
		
		
		linker[0].b=out_r[0].b.read();
		linker[0].p=out_r[0].p.read();
		linker[0].d=out_r[0].d.read();

		in_r[1].b.write(linker[0].b);
		in_r[1].c.write(linker[0].p);
		in_r[1].d.write(linker[0].d);		
		
		in[1].b=in_r[1].b.read();
		in[1].c=in_r[1].c.read();
		in[1].d=in_r[1].d.read();

		out[1] = pe[1].process(in[1]);
		
		out_r[1].b.write(out[1].b);
		out_r[1].p.write(out[1].p);
		out_r[1].d.write(1);		
		
		
		in[2].b=out_r[1].b.read();
		in[2].c=out_r[1].p.read();
		in[2].d=0;


		out[2] = pe[2].process(in[2]);

 
                //cout << "  valid = "<<valid<<" ";
		//if(valid){
			cout <<" OUT2="<<out[2].p<<" ";
			cout <<" OUT1="<<out[1].p<<" ";
			cout <<" IN1="<<in[1].b<<" ";
			cout <<" IN_r0="<<in_r[1].b.read()<<" ";
			cout <<" LINK0="<<linker[0].p<<" ";
			cout <<" OUT_r0="<<out_r[0].p.read()<<" ";
			cout <<" OUT0="<<out[0].p<<" ";
			cout <<" IN0="<<in[0].b<<" ";
		//}
            }
        }
	


};



int sc_main(int argc, char *argv[]) {
    // instantiate a producer and a consumer
    Producer tr("Transmitter1");
    Consumer rc("Receiver1");

    
	

	for(int i=0;i<4;i++){	
		pe[i].getconfig(i);
		pe[i].displayconfig();	
		pe[i].writeconfig();		
	}

	IN in[5];
	OUT out[4];
	
	in[0].b=1;
	in[0].c=0;
	in[0].d=0;
	
	
	/*for(int j=0;j<4;j++){
		out[j] = pe[j].process(in[j]);
		cout<<out[j].p<<endl;
	 
		in[j+1].b=out[j].b;
		in[j+1].c=out[j].p;
		in[j+1].d=0;
	}*/

    // instantiate a system clock: period 10 ms, duty cycle 50%
    //sc_set_time_resolution(1,SC_MS);
    sc_clock clk("Clock1", 10, SC_MS, 0.5);
    tr.clk(clk);
    rc.clk(clk);	

    // instantiate a wire to bind both modules to
    sc_signal<int> linktrrc;
    tr.value(linktrrc);
    rc.value(linktrrc);

    // start a 50 ms simulation of this system
    cout<<"clock 1 ------";
    sc_start(10, SC_MS);
    cout<<"clock 2 ------";//cout <<" OUT0="<<out[0].p<<" OUT1="<<out[1].p<<" ";           
    sc_start(10, SC_MS);
    cout<<"clock 3 ------";//cout <<" OUT0="<<out[0].p<<" OUT1="<<out[1].p<<" ";   
    sc_start(10, SC_MS);
    cout<<"clock 4 ------";//cout <<" OUT0="<<out[0].p<<" OUT1="<<out[1].p<<" ";   
    sc_start(10, SC_MS);
    cout<<"clock 5 ------";
    sc_start(10, SC_MS);
    cout<<"clock 6 ------";
    sc_start(10, SC_MS);
    cout<<"clock 7 ------";
    sc_start(10, SC_MS);
    cout<<"clock 8 ------";
    sc_start(10, SC_MS);
    cout<<"clock 9 ------";
    sc_start(10, SC_MS);
    cout<<"clock 10 ------";
    sc_start(10, SC_MS);
    cout<<"clock 11 ------";
    sc_start(10, SC_MS);
    cout<<"clock 12 ------";
    sc_start(10, SC_MS);
    cout<<"clock 13 ------";
    sc_start(10, SC_MS);
    cout<<"clock 14 ------";
    sc_start(10, SC_MS);
    cout<<"clock 15 ------";
    sc_start(10, SC_MS);
    cout<<"clock 16 ------";
    sc_start(10, SC_MS);

		
}

