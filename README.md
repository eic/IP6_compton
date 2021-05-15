# Geant simulation of Compton polarimeter in IP6 for the EIC at BNL

## Dependencies

- Geant 4
- ROOT 5/6

## Steps to checkout the repository and compile

<pre><code> git clone https://github.com/eic/compton.git </pre></code>
<pre><code> cd compton </pre></code>
<pre><code> mkdir build </pre></code>
<pre><code> cd build </pre></code>
<pre><code> cmake ../ </pre></code>
<pre><code> make </pre></code>
<pre><code> cd ../ </pre></code>

## Run
By default run the simulation from the main directory.

- Run as a batch job for a given number of events

<pre><code> ./build/run run.mac </pre></code>

- Or run with visualization

<pre><code> ./build/run </pre></code>

## Magnet files
All objects in the simulation are defined in the magnet file, change magFile in init_vic.mac to specify the needed magnet file.
