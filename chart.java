import java.time.LocalDate;
import java.time.ZoneOffset;
import java.util.Date;

import java.util.Scanner;
import java.io.*;
import org.jfree.data.time.*;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;
import javax.swing.WindowConstants;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.category.IntervalCategoryDataset;
import org.jfree.data.gantt.*;
import org.jfree.chart.plot.CategoryPlot;
import java.text.SimpleDateFormat;
import org.jfree.chart.axis.DateAxis;

// import org.jfree.data.gantt.TaskSeries;
// import org.jfree.data.gantt.TaskSeriesCollection;


public class chart extends JFrame 
{

   private static final long serialVersionUID = 1L;
   int totalTime;

   public chart(String title) 
   {
      super(title);
      // Read file and Create dataset
      IntervalCategoryDataset dataset = getCategoryDataset();
    
      // Create chart
      JFreeChart chart = ChartFactory.createGanttChart(
            "Process Execution", // Chart title
            "Process", // X-Axis Label
            "Timeline", // Y-Axis Label
            dataset);

      CategoryPlot plot=chart.getCategoryPlot();
      DateAxis axis= (DateAxis) plot.getRangeAxis();
      axis.setDateFormatOverride(new SimpleDateFormat("SSS"));
      axis.setMaximumDate(new Date(totalTime));

      ChartPanel panel = new ChartPanel(chart);
      setContentPane(panel);
   }

   private IntervalCategoryDataset getCategoryDataset() 
   {
      try
      {
         Scanner scanner = new Scanner(new File("processData.txt"));
         int levels=1,processes=0;
         totalTime=10;
         if(scanner.hasNextInt())
            levels=scanner.nextInt();
         if(levels==0)
            levels=1;

         if(scanner.hasNextInt())
            processes=scanner.nextInt();
         if(scanner.hasNextInt())
            totalTime=scanner.nextInt();

         // processes=4;
         // levels=3;

         TaskSeries [] series=new TaskSeries[levels];
         // TaskSeries series=new TaskSeries("aaa");
         for(int i=0;i<levels;i++)
         {
            series[i]=new TaskSeries("level "+i);
         }

         Task processTasks[][]=new Task[levels+2][processes+2];

         for(int i=0;i<levels;i++)
         {
            for(int j=0;j<processes;j++)
            {
               processTasks[i][j]=new Task("Process "+j,new SimpleTimePeriod(0,totalTime+2));
               processTasks[i][j].addSubtask(new Task("Process "+j,new SimpleTimePeriod((long)0,(long)0)));
            }
         }

         while(scanner.hasNextInt())
         {
            int pid=scanner.nextInt(),t_start=scanner.nextInt(),t_end=scanner.nextInt(),lvl=scanner.nextInt();
            // System.out.println(pid+" "+t_start+" "+t_end+" "+lvl);
            // Task a=new Task("Process "+1,new SimpleTimePeriod(0,10));
            if(pid<processes&&lvl<levels)
               processTasks[lvl][pid].addSubtask(new Task("Process "+pid,new SimpleTimePeriod((long)t_start,(long)t_end+1))); 

         }

         for(int i=0;i<levels;i++)
         {
            for(int j=0;j<processes;j++)
            {
               series[i].add(processTasks[i][j]);
            }
         }
        

         TaskSeriesCollection dataset = new TaskSeriesCollection(); 
         for(int i=0;i<levels;i++)
         {
            dataset.add(series[i]);
         }
         // dataset.add(series2);
         return dataset;
      }
      catch(Exception e)
      {
         System.out.println("error");
         System.out.println(e);
         // exit(0);
      }

      return null;
   }

   public static void main(String[] args) 
   {


      SwingUtilities.invokeLater(() -> 
      {
         chart example = new chart("Process Exception Gantt Chart");
         example.setSize(800, 400);
         example.setLocationRelativeTo(null);
         example.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
         example.setVisible(true);
      });
   }
}