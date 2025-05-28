class Base { }
class AnotherClass { }
class YetAnotherClass { }

class Derived : Base 
{ 
  public Derived() { } 
  public int IntField {get;} 
  public AnotherClass  ClassField {get; set;} 
  public bool do_work(string param1, double param2, YetAnotherClass param3){} 
  public override bool Equals(Object other) {} 
}
