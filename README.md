This is a small scale curl server in the works,
I'm building this to download videos onto my NAS remotely,
however you can download any file format.

As it currently stands, the client and server are in this repo.
This will change in the future to keep everything a little more clean.

TO USE:

  1.) run './Server <ip_address>' : if you are using localhost, 0.0.0.0 would work. If you are writing to a folder you don't own, use sudo or run as admin

  2.) run './Client <same_ip_address>'

FROM CLIENT:

  add <path/to/filename> <url> : This will add these to an array

  remove <path/to/filename> : This will remove item from array

  list : This will list all files in the array

  read <path/to/filename> : This will readback the url provided

  curl : This will go through the array and curl everything, if there are any errors with the curl it will tell you at the end.
