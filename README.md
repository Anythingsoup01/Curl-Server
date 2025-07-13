This is a small scale curl server in the works,
I'm building this to download videos onto my NAS remotely,
however you can download any file format.

As it currently stands, the client and server are in this repo.
This will change in the future to keep everything a little more clean.

TO USE:

  1.) run './Server <ip_address>' : if you are using localhost, 0.0.0.0 would work. If you are writing to a folder you don't own, use sudo or run as admin

  2.) run './Client <same_ip_address>'

FROM CLIENT:

  set cwd <path/to/folder> : This will set the path when curl is called, YOU CANNOT SET MULTIPLE!
  get cwd : This will return the set current working directory

  set extension <.extension> : This will set the extension when curl is called, YOU CANNOT SET MULTIPLE!
  get extension : This will return the current extension
  
  add "<file\ name>' <url> : This will add these to an array

  edit "<file\name>" <new_url> : This will allow you to edit the url field incase it's wrong or malformed

  remove <file\name> : This will remove item from array. Due to the nature of maps/unordered_maps all spaces are removed from the key.

  list : This will list all files in the array

  read <file\name> : This will readback the url provided

  curl : This will go through the array and curl everything, if there are any errors with the curl it will tell you at the end.
