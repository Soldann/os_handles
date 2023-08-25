Handle System

The handle allocation system prioritizes the following:
- be able to handle an arbitrary number of handles
- fast runtime with regard to large numbers of handles allocated at any one time

The binding system prioritizes the following criteria:
- handles an arbitrary number of handles
- can bind any data type to a handle
- fast access time
- moderately fast bind/free time

The API assumes the user is "moderately smart" and avoid doing something like calling `handle_bind` on a handle they do not know exists. This allows the handle system to use unions instead of structs to save on memory overhead at the expense of being less robust to undefined behaviour as a result of bad function calls. 
