A Minimalist 2D Game Engine Rendered Entirely in the Console (C++)
The application must be run with administrative privileges; otherwise, the console output may not function correctly.

This project is a custom-built 2D game engine that renders graphics directly in the console using character-based output.
It supports pixel-accurate rendering with extended Unicode characters and full color support. <br>
Originally intended to be a "TANK 1990"-style game, the project gradually evolved into a general-purpose console game engine developed for educational purposes.<br>
I plan to add Ethernet-based multiplayer support in the future.<br><br>

Technologies Used:<br>
    WinAPI — for efficient and flexible console input/output.<br><br>
    
Challenges:<br>
The main challenge I encountered was working with the WinAPI console. Understanding how it operates and identifying its limitations proved to be quite difficult.<br>
It took me several days of reading WinAPI documentation provided by Microsoft to fully grasp how WinAPI manages console input and output.<br>
Another significant challenge was understanding how certain algorithms work, such as Bresenham's algorithm, triangle rasterization and linear interpolation for rendering.<br>
