class Button
{
public:
    Button(int Pin_);
    ~Button(){}

    void init();
    bool isPressed(void);
private:
    int Pin;
};
