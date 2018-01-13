<h1 align="center"> i3-Battery-Warning </h1>

<h2>Introduction:</h2>
<p>This is a daemon written in C for battery warning. It uses notify-osd package for notifications. The installation script is made keeping i3 window manager in mind, but it can be used in any environment by keeping the compiled output in init scripts.</p>

<h2> Installation For i3</h2>
<p>Just run the install script.</p>

```bash
./install-i3-battery-warning
 ```
 
 <h2> Manual Installation </h2>
 <p>Compile the i3-battery-warning.cpp and place the executable so that it gets executed on startup.</p>
 
 ```bash
 g++ i3-battery-warning.cpp -o i3-battery-warning
 ```
 
