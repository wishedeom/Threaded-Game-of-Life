#pragma once

// Packages togeter a cells; current status and the status it will take on next turn
struct GridSquare
{
	bool is_alive;
	bool will_be_alive;
};
