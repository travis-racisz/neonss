#pragma once
class Points
{
private:
    int points = 0;
    int multiplier = 1;
public:

    int get_points() const { 
        return points;
    }
    int get_multiplier() const { 
        return multiplier;
    }

    void set_points(int increase){ 
        points += increase;
    } 
    void set_multiplier(int amount){ 
        multiplier = amount;
    }

};
