using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Trzecie_spotkanie
{
    class DemoClass<T>
    {
        public T ZwrocSamaSiebie(T obiekt)
        {
            return obiekt;
        }
    }

    interface InterfejsGeneryczny<T>
    {
        T Temp(T obiekt);
    }

    class PrzykladowaKlasa : InterfejsGeneryczny<string>
    {
        public string Temp(string obiekt)
        {
            return NotImplementedException();
        }
    }

    DemoClass<int> klasa = new DemoClass<int>();
    class Program
    {

        




        static void Main(string[] args)
        {
        }
    }
}
