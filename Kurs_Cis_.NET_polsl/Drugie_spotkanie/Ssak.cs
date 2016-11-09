using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Drugie_spotkanie
{
    class Ssak
    {
        public int age;
        protected string FirstName;

        private string LastName;
        public virtual void Name()
        {
            Console.WriteLine("Jestem ssakiem");
        }
        public Ssak(string FirstName, string LastName)
        {
            this.FirstName = FirstName;
            this.LastName = LastName;    
                }

        public override string ToString()
        {
            return this.FirstName + " " + this.LastName;
        }
    }

}
