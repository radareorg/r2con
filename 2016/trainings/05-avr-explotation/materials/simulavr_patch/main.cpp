/*
 ****************************************************************************
 *
 * simulavr - A simulator for the Atmel AVR family of microcontrollers.
 * Copyright (C) 2001, 2002, 2003   Klaus Rudolph
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 ****************************************************************************
 *
 *  $Id$
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <map>
#include <limits>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#ifndef _MSC_VER
#  include <getopt.h>
#else
#  include "../getopt/getopt.h"
#  define VERSION "(git-snapshot)"
#endif

#include "config.h"

#include "flash.h"
#include "avrdevice.h"
#include "avrfactory.h"
#include "avrsignature.h"
#include "avrreadelf.h"
#include "gdb.h"
#include "ui/ui.h"
#include "systemclock.h"
#include "ui/lcd.h"
#include "ui/keyboard.h"
#include "traceval.h"
#include "ui/scope.h"
#include "string2.h"
#include "helper.h"
#include "specialmem.h"
#include "irqsystem.h"
#include "avrdevice_impl.h"
#include "serialrx.h"
#include "serialtx.h"
#include "simulavr_info.h"


#include "dumpargs.h"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

const char *SplitOffsetFile(const char *arg,
                            const char *name,
                            int base,
                            unsigned long *offset)
{
    char *end;
    
    if(!StringToUnsignedLong(arg, offset, &end, base)) {
        cerr << name << ": offset is not a number" << endl;
        exit(1);
    }
    //position behind the "," or any other delimiter for the offset
    if(!*end) {
        cerr << name << ": argument ends before filename" << endl;
        exit(1);
    }
    if(*end != ',') {
        cerr << name << ": argument does not have comma before filename" << endl;
        exit(1);
    }
    ++end;
    if(!*end) {
        cerr << name << ": argument has comma but no filename" << endl;
        exit(1);
    }
    
    return end;
}

void addUARTBinding(AvrDevice *core, vector<string> v, bool rx){
    if(v.size() == 3){

        Net *net = new Net();

		if(rx){
		  SerialRxFile *serial = new SerialRxFile(v[0].c_str());
          serial->SetBaudRate(atoi(v[2].c_str()));
          net->Add(core->GetPin(v[1].c_str()));
          net->Add(serial->GetPin("rx"));
		}else{
		  SerialTxFile *serial = new SerialTxFile(v[0].c_str());
          serial->SetBaudRate(atoi(v[2].c_str()));
          net->Add(core->GetPin(v[1].c_str()));
          net->Add(serial->GetPin("tx"));
		}

    }else if(v.size() > 0){
        cerr << "Incorrect number of arguments for UART port specification." << endl;
        exit(1);
    }
}

const char Usage[] = 
    "AVR-Simulator Version " VERSION "\n"
    "-u                    run with user interface for external pin\n"
    "                      handling at port 7777\n"
    "-f --file <name>      load elf-file <name> for simulation in simulated target\n"
    "-x --rx0 <name,port,baud> \n"
    "                      bind uart0 rx port to file/stdio using baud"
    "-y --tx0 <name,port,baud> \n"
    "                      bind uart0 tx port to file/stdio using baud"
    "-z --rx1 <name,port,baud> \n"
    "                      bind uart1 rx port to file/stdio using baud"
    "-d --device <name>    simulate device <name> \n"
    "-g --gdbserver        listen for GDB connection on TCP port defined by -p\n"
    "-G --gdb-debug        listen for GDB connection and write debug info\n"
    "   --gdb-stdin        for use with GDB as 'target remote | ./simulavr'\n"
    "-m  <nanoseconds>     maximum run time of <nanoseconds>\n"
    "-M                    disable messages for bad I/O and memory references\n"
    "-p  <port>            use <port> for gdb server\n"
    "-t --trace <file>     enable trace outputs to <file>\n"
    "-l --linestotrace <number>\n"
    "                      maximum number of lines in each trace file.\n"
    "                      0 means endless. Attention: if you use gdb & trace, please use always 0!\n"
    "-n --nogdbwait        do not wait for gdb connection\n"
    "-F --cpufrequency     set the cpu frequency to <Hz> \n"
    "-s --irqstatistic     prints statistic informations about irq usage after simulation\n"
    "                      is stopped\n"
    "-W --writetopipe <offset>,<file>\n"
    "                      add a special pipe register to device at\n"
    "                      IO-Offset and opens <file> for writing\n"
    "-R --readfrompipe <offset>,<file>\n"
    "                      add a special pipe register to device at IO-offset\n"
    "                      and opens <file> for reading\n"
    "-a --writetoabort <offset>\n"
    "                      add a special register at IO-offset\n"
    "                      which aborts simulator run\n"
    "-e --writetoexit <offset>\n"
    "                      add a special register at IO-offset\n"
    "                      which exits simulator run\n"
    "-C --core-dump <name> dump a core memory image <name> to file on exit\n"
    "-v --verbose          output some hints to console\n"
    "-T --terminate <label> or <address>\n"
    "                      stops simulation if PC runs on <label> or <address>\n"
    "-B --breakpoint <label> or <address>\n"
    "                      same as -T for backward compatibility\n"
    "-c <tracing-option>   Enables a tracer with a set of options. The format for\n"
    "                      <tracing-option> is:\n"
    "                      <tracer>[:further-options ...]\n"
    "-o <trace-value-file> Specifies a file into which all available trace value names\n"
    "                      will be written.\n"
    "-V --version          print out version and exit immediately\n"
    "-h --help             print this help\n"
    "\n";

int main(int argc, char *argv[]) {
    int c;
    bool gdbserver_flag = 0;
    string coredumpfile("unknown");
    string filename("unknown");
    string devicename("unknown");
    string tracefilename("unknown");
    long global_gdbserver_port = 1212;
    int global_gdb_debug = 0;
    bool globalWaitForGdbConnection = true; //please wait for gdb connection
    int userinterface_flag = 0;
    unsigned long long fcpu = 0;
    unsigned long long maxRunTime = 0;
    unsigned long long linestotrace = 1000000;
    UserInterface *ui;
    
    unsigned long writeToPipeOffset = 0x20;
    unsigned long readFromPipeOffset = 0x21;
    unsigned long writeToAbort = 0;
    unsigned long writeToExit = 0;
    string readFromPipeFileName = "";
    string writeToPipeFileName = "";

    vector<string> uart0rxArgs;
    vector<string> uart0txArgs;
    vector<string> uart1rxArgs;
    
    vector<string> terminationArgs;
    
    vector<string> tracer_opts;
    bool tracer_dump_avail = false;
    string tracer_avail_out;
    
    while (1) {
        //int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"file", 1, 0, 'f'},
            {"device", 1, 0, 'd'},
            {"gdbserver", 0, 0, 'g'},
            {"gdb-debug", 0, 0, 'G'},
            {"debug-gdb", 0, 0, 'G'},
            {"linestotrace", 1, 0, 'l'},
            {"maxruntime", 1, 0, 'm'},
            {"nogdbwait", 0, 0, 'n'},
            {"trace", 1, 0, 't'},
	    {"rx0", 1, 0, 'x'},
            {"version", 0, 0, 'V'},
            {"cpufrequency", 1, 0, 'F'},
            {"readfrompipe", 1, 0, 'R'},
            {"writetopipe", 1, 0, 'W'},
            {"writetoabort", 1, 0, 'a'},
            {"writetoexit", 1, 0, 'e'},
            {"verbose", 0, 0, 'v'},
            {"terminate", 1, 0, 'T'},
            {"breakpoint", 1, 0, 'B'},
            {"core-dump", 1, 0, 'C'},
            {"irqstatistic", 0, 0, 's'},
            {"help", 0, 0, 'h'},
            {0, 0, 0, 0}
        };
        
        c = getopt_long(argc, argv, "a:e:f:d:gGm:p:t:x:y:z:uhvnisF:R:W:VT:B:c:C:o:l:", long_options, &option_index);
        if(c == -1)
            break;
        
        switch(c) {
	    case 'x':
		split(string(optarg), ',', uart0rxArgs);
		break;

	    case 'y':
		split(string(optarg), ',', uart0txArgs);
		break;

	    case 'z':
		split(string(optarg), ',', uart1rxArgs);
		break;

            case 'B':
            case 'T':
                terminationArgs.push_back(optarg);
                break;
            
            case 'v':
                global_verbose_on = 1;
                break;
            
            case 'R': //read from pipe 
                readFromPipeFileName = 
                    SplitOffsetFile(optarg, "readFromPipe", 16, &readFromPipeOffset);
                break;
            
            case 'W': //write to pipe
                writeToPipeFileName = 
                   SplitOffsetFile(optarg, "writeToPipe", 16, &writeToPipeOffset);
                break;
            
            case 'a': // write to abort
                if(!StringToUnsignedLong(optarg, &writeToAbort, NULL, 16)) {
                    cerr << "writeToAbort is not a number" << endl;
                    exit(1);
                }
                break;
            
            case 'e': // write to exit
                if(!StringToUnsignedLong(optarg, &writeToExit, NULL, 16)) {
                    cerr << "writeToExit is not a number" << endl;
                    exit(1);
                }
                break;
            
            case 'F':
                if(!StringToUnsignedLongLong(optarg, &fcpu, NULL, 10)) {
                    cerr << "frequency is not a number" << endl;
                    exit(1);
                }
                if(fcpu == 0) {
                    cerr << "frequency is zero" << endl;
                    exit(1);
                }
                break;

            case 'l':
                if(!StringToUnsignedLongLong( optarg, &linestotrace, NULL, 10)) {
                    cerr << "linestotrace is not a number" << endl;
                    exit(1);
                }
                break;

            case 'm':
                if(!StringToUnsignedLongLong( optarg, &maxRunTime, NULL, 10)) {
                    cerr << "maxRunTime is not a number" << endl;
                    exit(1);
                }
                if(maxRunTime == 0) {
                    cerr << "maxRunTime is zero" << endl;
                    exit(1);
                }
                avr_message("Maximum Run Time: %lld", maxRunTime);
                break;
            
            case 'u':
                avr_message("Run with User Interface at Port 7777");
                userinterface_flag = 1;
                break;
            
            case 'f':
                avr_message("File to load: %s", optarg);
                filename = optarg;
                break;
            
            case 'd':
                devicename = optarg;
                break;
            
            case 'g':
                avr_message("Running as gdb-server");
                gdbserver_flag = 1;
                break;
            
            case 'G':
                avr_message("Running with debug information from gdbserver");
                global_gdb_debug = 1;
                gdbserver_flag = 1;
                break;
            
            case 'p':
                if(!StringToLong( optarg, &global_gdbserver_port, NULL, 10)) {
                    cerr << "GDB Server Port is not a number" << endl;
                    exit(1);
                }
                avr_message("Running on port: %ld", global_gdbserver_port);
                break;
            
            case 't':
                avr_message("Running in Trace Mode with maximum %lld lines per file",
                            linestotrace);

                sysConHandler.SetTraceFile(optarg, linestotrace);
                break;
            
            case 'V':
                cout << "SimulAVR " << VERSION << endl
                     << "See documentation for copyright and distribution terms" << endl
                     << endl;
                exit(0);
                break;
            
            case 'n':
                cout << "We will NOT wait for a gdb connection, "
                        "simulation starts now!" << endl;
                globalWaitForGdbConnection = false;
                break;
            
            case 'c':
                tracer_opts.push_back(optarg);
                break;
            
            case 'o':
                tracer_dump_avail = true;
                tracer_avail_out = optarg;
                break;
             
            case 's':
                enableIRQStatistic = true;
                break;
            
            case 'C':
                avr_message("Write core dump on exit to file: %s", optarg);
                coredumpfile = optarg;
                break;
            
            default:
                cout << Usage
                     << "Supported devices:" << endl
                     << AvrFactory::supportedDevices() << endl;
                exit(0);
        }
    }
    
    /* get dump manager and inform it, that we have a single device application */
    DumpManager *dman = DumpManager::Instance();
    dman->SetSingleDeviceApp();
    
    /* check, if devicename is given or get it out from elf file, if given */
    unsigned int sig;
    char *new_devicename = (char *)malloc(1024); // can't be static

    strncpy(new_devicename, devicename.c_str(), 1024);
    if(filename != "unknown") {
        sig = ELFGetDeviceNameAndSignature(filename.c_str(), new_devicename);
    }

    /* now we create the device and set device name and signature */
    AvrDevice *dev1 = AvrFactory::instance().makeDevice(new_devicename);
    dev1->SetDeviceNameAndSignature(new_devicename, sig);
    free(new_devicename);

    addUARTBinding(dev1, uart0rxArgs, true);
    addUARTBinding(dev1, uart0txArgs, false);
    addUARTBinding(dev1, uart1rxArgs, true);

    /* We had to wait with dumping the available tracing values
      until the device has been created! */
    if(tracer_dump_avail) {
        ShowRegisteredTraceValues(tracer_avail_out);
        exit(0);
    }
    
    /* handle DumpTrace option */
    SetDumpTraceArgs(tracer_opts, dev1);
    
    if(!gdbserver_flag && filename == "unknown") {
        cerr << "Specify either --file <executable> or --gdbserver (or --gdb-stdin)" << endl;
        exit(1);
    }
    
    //if we want to insert some special "pipe" Registers we could do this here:
    if(readFromPipeFileName != "") {
        avr_message("Add ReadFromPipe-Register at 0x%lx and read from file: %s",
                    readFromPipeOffset, readFromPipeFileName.c_str());
        dev1->ReplaceIoRegister(readFromPipeOffset,
            new RWReadFromFile(dev1, "FREAD", readFromPipeFileName.c_str()));
    }
    
    if(writeToPipeFileName != "") {
        avr_message("Add WriteToPipe-Register at 0x%lx and write to file: %s",
                    writeToPipeOffset, writeToPipeFileName.c_str());
        dev1->ReplaceIoRegister(writeToPipeOffset,
            new RWWriteToFile(dev1, "FWRITE", writeToPipeFileName.c_str()));
    }
    
    if(writeToAbort) {
        avr_message("Add WriteToAbort-Register at 0x%lx", writeToAbort);
        dev1->ReplaceIoRegister(writeToAbort, new RWAbort(dev1, "ABORT"));
    }
    
    if(writeToExit) {
        avr_message("Add WriteToExit-Register at 0x%lx", writeToExit);
        dev1->ReplaceIoRegister(writeToExit, new RWExit(dev1, "EXIT"));
    }
    
    if(filename != "unknown" ) {
        dev1->Load(filename.c_str());
        dev1->Reset(); // reset after load data from file to activate fuses and lockbits
    }
    
    //if we have a file we can check out for termination lines.
    vector<string>::iterator ii;
    for(ii = terminationArgs.begin(); ii != terminationArgs.end(); ii++) {
        avr_message("Termination or Breakpoint Symbol: %s", (*ii).c_str());
        dev1->RegisterTerminationSymbol((*ii).c_str());
    }
    
    //if not gdb, the ui will be master controller :-)
    ui = (userinterface_flag == 1) ? new UserInterface(7777) : NULL;
    
    if(fcpu != 0)
        dev1->SetClockFreq((SystemClockOffset)1000000000 / fcpu); // time base is 1ns!

    if(dev1->GetClockFreq() == 0) {
        avr_warning("Clock frequency not given, defaulting to 4000000. "
                    "Use -F | --cpufrequency FREQ or insert a "
                    "SIMINFO_CPUFREQUENCY(freq) macro into your source %s",
                    "to specify it.");
        dev1->SetClockFreq((SystemClockOffset)1000000000 / 4000000);
    }
    avr_message("Running with CPU frequency: %1.3lf MHz (%lld Hz)\n",
                (double)1000 / dev1->GetClockFreq(),
                (unsigned long long)1000000000 / dev1->GetClockFreq());

    if(sysConHandler.GetTraceState())
        dev1->trace_on = 1;
    
    dman->start(); // start dump session
    
    long steps = 0;
    if(gdbserver_flag == 0) { // no gdb
        SystemClock::Instance().Add(dev1);
        if(maxRunTime == 0) {
            steps = SystemClock::Instance().Endless();
            cout << "SystemClock::Endless stopped" << endl
                 << "number of cpu cycles simulated: " << dec << steps << endl;
        } else {                                           // limited
            steps = SystemClock::Instance().Run(maxRunTime);
            cout << "Ran too long. Terminated after "
                 << dec << steps << " cpu cycles simulated." << endl;
        }
        Application::GetInstance()->PrintResults();
    } else { // gdb should be activated
        avr_message("Waiting for gdb connection ...");
        GdbServer gdb1(dev1, global_gdbserver_port, global_gdb_debug, globalWaitForGdbConnection);
        SystemClock::Instance().Add(&gdb1);
        SystemClock::Instance().Endless();
        if(global_verbose_on) {
            cout << "SystemClock::Endless stopped" << endl
                 << "number of cpu cycles simulated: " << dec << steps << endl;
            Application::GetInstance()->PrintResults();
        }
    }
    
    dman->stopApplication(); // stop dump session. Close dump files, if necessary
    
    if(coredumpfile != "unknown") {
        avr_message("write core dump file ...");
        WriteCoreDump(coredumpfile, dev1);
    }

    // delete ui and device
    delete ui;
    delete dev1;
    
    return 0;
}

