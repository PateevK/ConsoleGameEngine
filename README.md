A Minimalist 2D Game Engine Rendered Entirely in the Console (C++)

This project is a custom-built 2D game engine that renders graphics directly in the console using character-based output. 
It supports pixel-accurate rendering with extended Unicode characters and full color support.
Originally intended to be a "TANK 1990"-style game, the project gradually evolved into a general-purpose console game engine developed for educational purposes.
I plan to add Ethernet-based multiplayer support in the future.

Technologies Used:
    WinAPI — for efficient and flexible console input/output.
    
Challenges:
The main challenge I encountered was working with the WinAPI console. Understanding how it operates and identifying its limitations proved to be quite difficult.
It took me several days of reading WinAPI documentation provided by Microsoft to fully grasp how WinAPI manages console input and output.
Another significant challenge was understanding how certain algorithms work, such as Bresenham's algorithm, triangle rasterization and linear interpolation for rendering.
