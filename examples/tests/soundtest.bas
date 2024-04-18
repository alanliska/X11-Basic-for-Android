' Test the SOUND command. It Sounds the internal speaker.
' permissions for /dev/console have to be set correctly if xterm
' is used.     (c) Markus Hoffmann 2003
'
DO
  READ f
  EXIT IF f=-1
  READ p
  SOUND 1,f
  PAUSE p/700
LOOP
SOUND 1,0
QUIT

DATA 100,100
DATA 200,100
DATA 300,100
DATA 400,100
DATA 500,100
DATA 600,100
DATA 100,100
DATA 200,100
DATA 300,100
DATA 400,100
DATA 500,100
DATA 600,100
DATA 500,100
DATA 400,100
DATA 300,100
DATA 200,100
DATA 100,100
DATA 100,100
DATA 0,600
DATA 400,50
DATA 00,50
DATA 400,50
DATA 00,50
DATA 400,50
DATA 00,50
DATA 600,80
DATA 00,50
DATA 400,80
DATA 00,50
DATA -1,-1
