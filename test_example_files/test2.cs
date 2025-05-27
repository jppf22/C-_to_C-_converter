public class User : Person
{
    private int id;
    protected string email;

    public string Name { get; set; }
    public int Age { get; set; }

    public User(string name, int age)
    {
        Name = name;
        Age = age;
    }

    public void PrintInfo()
    {
        Console.WriteLine($"Name: {Name}, Age: {Age}");
    }

    protected override void Greet()
    {
        Console.WriteLine("Hello from User!");
    }
}