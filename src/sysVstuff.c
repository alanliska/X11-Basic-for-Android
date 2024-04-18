/* sysVstuff.c         (c) Markus Hoffmann   */

/* Semaphoren, Shared- Memory Objekte und Message Queues */

/* This file is part of X11BASIC, the basic interpreter for Unix/X
 * ============================================================
 * X11BASIC is free software and comes with NO WARRANTY - read the file
 * COPYING for details
 */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "defs.h"
#include "x11basic.h"
#include "xbasic.h"
#include "memory.h"
#if defined WINDOWS || defined ANDROID
#define EX_OK 0
#else
#include <sysexits.h>
#endif
 
#include <sys/types.h>

#ifndef WINDOWS
#include <sys/ipc.h>
#if 0
#include <sys/msg.h>
#endif
#ifndef ANDROID
#include <sys/sem.h>
#include <sys/shm.h>
#endif
#else
#define key_t int
#endif
#ifdef __APPLE__
#define key_t int
#endif


#define FALSE 0
#define TRUE (!FALSE)

#define MAX_SEND_SIZE 80
#define SEM_RESOURCE_MAX         1        /* Initial value of all semaphores */

void io_error(int,char *);

#if 0
struct mymsgbuf {
        long mtype;
        char mtext[MAX_SEND_SIZE];
};

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text);
void read_message(int qid, struct mymsgbuf *qbuf, long type);
void remove_queue(int qid);
void change_queue_mode(int qid, char *mode);
#endif
#if 0
static void opensem(int *sid, key_t key);
static void createsem(int *sid, key_t key, int members);
void locksem(int sid, int member);
void unlocksem(int sid, int member);
void removesem(int sid);
static unsigned short get_member_count(int sid);
static int getval(int sid, int member);
#endif
void dispval(int sid, int member);
void changemode(int sid, char *mode);
void shm_free(int);
void change_shm_mode(int, char *);
int shm_malloc(size_t,key_t);
#if 0

int open_msg(key_t key) {
  int msgqueue_id;
#ifndef WINDOWS
      /* Open the queue - create if necessary */
        if((msgqueue_id = msgget(key, IPC_CREAT|0660)) == -1) {
                 io_error(errno,"open_msg");    /* Message error.*/ 
                 return(-1);
        }
#endif	
  return(msgqueue_id);
}

int peek_message( int qid, long type ) {
  int     result, length;
#ifndef WINDOWS
         if((result = msgrcv( qid, NULL, 0, type,  IPC_NOWAIT)) == -1)
         { if(errno == E2BIG) return(TRUE); }
#endif
        return(FALSE);
}

void change_queue_mode(int qid, char *mode) {
#ifndef WINDOWS	
        struct msqid_ds myqueue_ds;
        msgctl(qid, IPC_STAT, &myqueue_ds);            /* Get current info */
        sscanf(mode, "%ho", &myqueue_ds.msg_perm.mode);/* Convert and load the mode */
        msgctl(qid, IPC_SET, &myqueue_ds);             /* Update the mode */
#endif
}

void send_message(int qid, struct mymsgbuf *qbuf, long type, char *text) {
        /* Send a message to the queue */
#if DEBUG
        printf("Sending a message ...\n");
#endif
        qbuf->mtype = type;
        strcpy(qbuf->mtext, text);
#ifndef WINDOWS
        if((msgsnd(qid, (struct msgbuf *)qbuf,
                strlen(qbuf->mtext)+1, 0)) ==-1) {
               io_error(errno,"send_message");    /* Message konnte nicht versandt werden.*/ 
        }
#endif	
}


void read_message(int qid, struct mymsgbuf *qbuf, long type)
{
        /* Read a message from the queue */
        qbuf->mtype = type;
#ifndef WINDOWS
        msgrcv(qid, (struct msgbuf *)qbuf, MAX_SEND_SIZE, type, 0);
#endif
#if DEBUG
        printf("Type: %ld Text: %s\n", qbuf->mtype, qbuf->mtext);
#endif
}


void remove_queue(int qid) /* Remove the msg-queue */ {
#ifndef WINDOWS
        msgctl(qid, IPC_RMID, 0);
#endif
}


#if 0
static int opensem( key_t key) {   /* Open the semaphore set - do not create! */
  int sid;
  if((*sid = semget(key, 0, 0666)) == -1) printf("Semaphore set does not exist!\n");
  return(sid);
}


static void createsem(int *sid, key_t key, int members) {
  int cntr;
  union semun semopts;

  if(members > SEMMSL) {
	  printf("Sorry, max number of semaphores in a set is %d\n",SEMMSL);
	  return;
  }
  printf("Attempting to create new semaphore set with %d members\n",members);
  if((*sid = semget(key, members, IPC_CREAT|IPC_EXCL|0666))== -1) {
                fprintf(stderr, "Semaphore set already exists!\n");
                return;
  }


        semopts.val = SEM_RESOURCE_MAX;


        /* Initialize all members (could be done with SETALL) */
        for(cntr=0; cntr<members; cntr++)
                semctl(*sid, cntr, SETVAL, semopts);
}

void locksem(int sid, int member)
{
        struct sembuf sem_lock={ 0, -1, IPC_NOWAIT};


        if( member<0 || member>(get_member_count(sid)-1))
        {
                 fprintf(stderr, "semaphore member %d out of range\n", member);
                 return;
        }


        /* Attempt to lock the semaphore set */
        if(!getval(sid, member)) {
                 fprintf(stderr, "Semaphore resources exhausted (no lock)!\n");
                 return;
        }

        sem_lock.sem_num = member;

        if((semop(sid, &sem_lock, 1)) == -1) {
                 fprintf(stderr, "Lock failed\n");
                 return;
        } else printf("Semaphore resources decremented by one (locked)\n");

        dispval(sid, member);
}
void unlocksem(int sid, int member)
{
        struct sembuf sem_unlock={ member, 1, IPC_NOWAIT};
        int semval;


        if( member<0 || member>(get_member_count(sid)-1))
        {
                 fprintf(stderr, "semaphore member %d out of range\n", member);
                 return;
        }


        /* Is the semaphore set locked? */
        semval = getval(sid, member);
        if(semval == SEM_RESOURCE_MAX) {
                 fprintf(stderr, "Semaphore not locked!\n");
                 return;
        }
        sem_unlock.sem_num = member;


        /* Attempt to lock the semaphore set */
        if((semop(sid, &sem_unlock, 1)) == -1) {
                fprintf(stderr, "Unlock failed\n");
                return;
        } else printf("Semaphore resources incremented by one (unlocked)\n");
        dispval(sid, member);
}

void removesem(int sid){
  semctl(sid, 0, IPC_RMID, 0);
}


static unsigned short get_member_count(int sid) {
        union semun semopts;
        struct semid_ds mysemds;


        semopts.buf = &mysemds;


        /* Return number of members in the semaphore set */
        return(semopts.buf->sem_nsems);
}
static int getval(int sid, int member) {
        int semval;
	
        semval = semctl(sid, member, GETVAL, 0);
        return(semval);
}


void changemode(int sid, char *mode) {
  int rc;
  union semun semopts;
  struct semid_ds mysemds;

        /* Get current values for internal data structure */

        semopts.buf = &mysemds;
        rc = semctl(sid, 0, IPC_STAT, semopts);
        if (rc == -1) {
                 perror("semctl");
                 return;
        }

        printf("Old permissions were %o\n", semopts.buf->sem_perm.mode);


        /* Change the permissions on the semaphore */
        sscanf(mode, "%ho", &semopts.buf->sem_perm.mode);


        /* Update the internal data structure */
        semctl(sid, 0, IPC_SET, semopts);
}



void dispval(int sid, int member) {
        int semval;
        semval = semctl(sid, member, GETVAL, 0);
        printf("semval for member %d is %d\n", member, semval);
}
#endif
#endif

#if defined WINDOWS || defined __CYGWIN__ || defined ANDROID
  #define shmget(a,b,c) (-1)
  #define shmat(a,b,c) (-1)
  #define shmdt(a) (-1)
  #define shmctl(a,b,c) (-1)
  #ifndef IPC_CREAT
    #define IPC_CREAT 0
    #define IPC_EXCL 0
    #define IPC_RMID 0
    #define IPC_STAT 0
    #define IPC_SET 0
  #endif
#endif


int shm_malloc(size_t segsize, key_t key) {
  int shmid=shmget(key, segsize, IPC_CREAT|IPC_EXCL|0666);
  /* Open the shared memory segment - create if necessary */
  if(shmid==-1) {
    /* Segment probably already exists - try as a client */
    shmid=shmget(key,segsize,0);
  }
  if(shmid==-1) io_error(errno,"SHM_MALLOC");    /* shm_malloc error.*/ 
  return(shmid);
}

int shm_attach(int shmid) {
  int r=POINTER2INT(shmat(shmid,0,0));
  if(r==-1) io_error(errno,"SHM_ATTACH");    /* shm_attach error.*/ 
  return(r);
}

int shm_detatch(const void *shmaddr) {
  if(shmdt(shmaddr)==-1) return(errno);
  return(0);
}

void shm_free(int shmid) {        /*  mark for deletion*/
  int r=shmctl(shmid, IPC_RMID, 0);
  if(r==-1) io_error(errno,"SHM_FREE");
}

#if 0
void change_shm_mode(int shmid, char *mode) {
        struct shmid_ds myshmds;

        /* Get current values for internal data structure */
        shmctl(shmid, IPC_STAT, &myshmds);
      /*  printf("Old permissions were: %o\n", myshmds.shm_perm.mode); */
        /* Convert and load the mode */
        sscanf(mode, "%o", &myshmds.shm_perm.mode);
        shmctl(shmid, IPC_SET, &myshmds);  /* Update the mode */
/*        printf("New permissions are : %o\n", myshmds.shm_perm.mode);*/
}
#endif
