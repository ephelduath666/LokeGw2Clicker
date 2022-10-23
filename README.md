# LOKE GW2 Clicker

This is an Auto Clicker tailormade for Goblers in Guild Wars 2. It should be within GW2 TOS, as that states autoclickers may be used for this purpose.

Download latest binary and start it up.

Hw to use:
1. Select the desired profile (tuned to goblers and research kits)
2. Open your inventory and move your mouse to the centre on the gobler or tool you want to use
3. Press ALT+CTRL+G and the clicker respons with a short beep
4. Press the hot key again to stop the clicker, or move your mouse outside of the icon, and it stops automatically.
   Two short beeps indicates that the clicker has stopped clicking.

The profiles are
- Candy Corn Gobler
  This has a random interval between 5 and 5.2 seconds

- Snowflake Gobler
  This clicks at a rate of apx 100 ms (+/- some random small value)
  
- Get rid of stack
  This clicks away at 50ms intervals (+/- some random small value)

Upcoming changes:

- More profiles, if needed
- Refactor code so it won't be so hacky, but it was only made in a hurry so I could get the gobler bonuses going during Alliance Beta

The code is written in C++ (MSVC 17) only using Win32 controls for minimum footprint. In the future it might be re-written, but I doubt it. 
It really has a developer frontend, and no fancy graphics at all.
