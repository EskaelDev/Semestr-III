using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Drugie_spotkanie
{
   abstract class Ssak : ISsak
    {
        public int age;
        protected string FirstName;

        private string LastName;
        public abstract void Name();

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
