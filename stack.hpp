#define MAXSTACK 100


typedef Token * Object ;

class stack {
public:
  stack();
  ~stack();
  Object Top() const;
  bool isEmpty() const;
  Object Pop();
  void Push(Object item);
  
private:
  Object items[MAXSTACK];
  int top;
};


