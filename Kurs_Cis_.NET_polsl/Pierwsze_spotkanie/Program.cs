using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pierwsze_spotkanie
{
    class Dzik
    {
        public string name;
        public Dzik(string nazwa)
        {
            name = nazwa;
        }
    }

    public class Person
    {
        // Propercje
           public int age { get; set; } // Zapewnia hermetyzacje
        public string Name { get; set; }
    }
    class Program
    {
        static void Main(string[] args)
        {
            string x = "kameha";
            var Boear = new Dzik("TUTUTUTUTUT");

            var osoba = new Person;

            osoba.age = 5;

            Console.Write("Test textu");
            Console.Write(Boear.name);
            Console.ReadKey();
            
        }
    }



}
