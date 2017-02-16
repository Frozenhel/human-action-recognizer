#pragma once

enum DailyAction
{
	drinking = 1,
	eating = 2,
	reading = 3,
	phoning = 4,
	writing = 5,
	using_computer = 6,	
	vacuuming = 7,
	celebrating = 8,
	doing_nothing = 9,
	throwing = 10,
	playing_games = 11,
	laying_down = 12,
	walking_hidden = 13,
	playing_guitar = 14,
	stand_up = 15,
	sit_down = 16
};

inline const char* ToString(DailyAction action)
{
	switch (action)
	{
		case drinking:			return "Drinking";
		case eating:			return "Eating";
		case reading:			return "Reading";
		case phoning:			return "Phoning";
		case writing:			return "Writing";
		case using_computer:	return "Using computer";
		case vacuuming:			return "Vacuuming";
		case celebrating:		return "Celebrating";
		case doing_nothing:		return "Doing nothing";
		case throwing:			return "Throwing ball";
		case playing_games:		return "Playing games";
		case laying_down:		return "Laying down";
		case walking_hidden:	return "Walking";
		case playing_guitar:	return "Playing guitar";
		case stand_up:			return "Stand up";
		case sit_down:			return "Sit down";
		default:				return "[Unknown action]";
	}
}