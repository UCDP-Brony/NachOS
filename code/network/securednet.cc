// securednet.cc 
//	Attempt to provide secured primitives to allow safe communication (packet-loss-wise) between two Nachos
//
//	Two caveats:
//	  1. Two copies of Nachos must be running, with machine ID's 0 and 1:
//		./nachos -m 0 -o3 1 &
//		./nachos -m 1 -o4 0 &
//
//	  2. You need an implementation of condition variables,
//	     which is *not* provided as part of the baseline threads 
//	     implementation.  The Post Office won't work without
//	     a correct implementation of condition variables.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "network.h"
#include "post.h"
#include "interrupt.h"

#define MAXREEMISSIONS		20

#define TEMPO				2


// Server
// 
void Receveur(int farAddr){
	PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;
    const char *ack = "Got it!";
    char buffer[MaxMailSize];
	//start looping waiting for a packet
	while(1){
			postOffice->Receive(1, &inPktHdr, &inMailHdr, buffer);
			printf("Got \"%s\" from %d, box %d\n",buffer,inPktHdr.from,inMailHdr.from);
			fflush(stdout);
			outPktHdr.to = inPktHdr.from;
			outMailHdr.to = inMailHdr.from;
			outMailHdr.length = strlen(ack) + 1;
			postOffice->Send(outPktHdr, outMailHdr, ack);
	}
	//not reached
	interrupt->Halt();
}

// Client
void Emetteur(int farAddr){
	
	PacketHeader outPktHdr, inPktHdr;
    MailHeader outMailHdr, inMailHdr;
    const char *data = "Hello there!";
    char buffer[MaxMailSize];
	// construct packet, mail header for original message
	// To: destination machine, mailbox 0
	// From: our machine, reply to: mailbox 1
	int tentative = 0;
	bool success = false;
	while (tentative<MAXREEMISSIONS && !success){
		
		
		outPktHdr.to = farAddr;		
		outMailHdr.to = 1;
		outMailHdr.from = 0;
		outMailHdr.length = strlen(data) + 1;
		postOffice->Send(outPktHdr, outMailHdr, data);
		
		if(postOffice->ReceiveReliable(0, &inPktHdr, &inMailHdr, buffer,TEMPO)){
			success = true;
		}
		tentative++;
		printf("Tentative numéro %i \n",tentative);
		fflush(stdout);
	}
	if (!success){
		printf("Failed to send the message : too many messages sent without ack \n");
	}
	else{
		printf("Got \"%s\" from %d\n",buffer,inPktHdr.from);
	}
	fflush(stdout);
	interrupt->Halt();
}

