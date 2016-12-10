using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;



namespace Piate_spotkanie
{

    


    class Program
    {
        public static void Example1()
        {
            //Define and run task
            Thread.CurrentThread.Name = "Main";

            // Output a message from the calling thread
            Task taskA = Task.Run(
                () => Console.WriteLine("Hello from taskA."));

            Console.WriteLine("Hello from thread '{0}'.", Thread.CurrentThread.Name);


            Task taskB = new Task(() => Console.WriteLine("Hello from taskB."));

            taskB.Start();
            taskA.Wait();

            taskB.Wait();

        }

        public static void Example2()
        {
            Task<double>[] taskArray = {Task<double>.Factory.StartNew(()=> DoComputation(1.0)),
                                        Task<double>.Factory.StartNew(()=> DoComputation(100.0)),
                                        Task<double>.Factory.StartNew(()=> DoComputation(1000.0))};

            var resault = new double[taskArray]
        }


        static void Main(string[] args)
        {
            Example1();
            Console.ReadKey();
        }
    }
}
