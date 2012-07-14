MSP430-controlled-mobile
========================

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
At the beginning of the year I had to go to San Francisco for business and I found a neat airplane mobile for our unborn son. After he was born Stacy hung it from the light in his room and whenever he was upset she would take him into his room to "watch the airplanes". She would give them a toss and he would watch them spin around. A while later she said that I needed to automate them so that the mobile would turn by itself.

This is what I came up with. It is mounted on the light fixture and has the mobile hanging from underneath. You activate it by pointing any IR remote at it and pressing a button. The mobile will spin for five minutes and then shut off. You can turn it off manually by pointing the remote at the mobile and pressing a button. An MSP430 microcontroller provides the processing and the circuit is powered by two "D" cell batteries. The batteries should last for around 6 months. A "hobby" servo modified for continuous rotation is what actually turns the mobile. All programming was done in C using MSP-GCC under Linux.

Ordinarily a servo could never move such a heavy load (the mobile is at least 2lbs). The secret to that success is the rope connecting the mobile to the servo. The servo will start to spin and that energy is stored in the coiled rope until it is enough to overcome the weight of the mobile. This also helps absorb the start and start forces that would normally rip the gear train to shreds.

I'm not responsible if you decide to copy this project and anything bad happens. It may very well burst into flames, catch your house on fire, and give you BO. Seriously, it's not my fault.

Here is the mobile in action: http://www.youtube.com/watch?v=M4d147vko8w