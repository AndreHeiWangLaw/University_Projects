package coen352.a3;

import static org.junit.Assert.assertTrue;
import static org.junit.jupiter.api.Assertions.*;

import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;


/** Source code example for "A Practical Introduction to Data
    Structures and Algorithm Analysis, 3rd Edition (Java)" 
    by Clifford A. Shaffer
    Copyright 2008-2011 by Clifford A. Shaffer
*/

// Graph operation main function.
// To use: java -ea GraphTest <graphfile>

import java.io.*;
import java.util.*;

public class GraphTester
   
{

	static final int UNVISITED = 0;
	static final int VISITED = 1;

	
	
	// Create a graph from file
	static ADTGraph createGraph(BufferedReader file, ADTGraph G) 
	throws IOException 
	{
			  String line = null;
			  StringTokenizer token;
			  boolean undirected = false;
			  int i, v1, v2, weight;
			
			  assert (line = file.readLine()) != null :
			         "Unable to read number of vertices";
			  while(line.charAt(0) == '#')
			  assert (line = file.readLine()) != null :
			         "Unable to read number of vertices";
			  token = new StringTokenizer(line);
			  
			  int n = Integer.parseInt(token.nextToken());
			  G.Init(n);
			  
			  for (i=0; i<n; i++)
			    G.setMark(i, UNVISITED);
			  
			  assert (line = file.readLine()) != null :
			         "Unable to read graph type";
			  
			  if (line.charAt(0) == 'U')
			    undirected = true;
			  else if (line.charAt(0) == 'D')
			    undirected = false;
			  else assert false : "Bad graph type: " + line;
			
			  // Read in edges
			  while((line = file.readLine()) != null) {
				token = new StringTokenizer(line);
			    v1 = Integer.parseInt(token.nextToken());
			    v2 = Integer.parseInt(token.nextToken());
			    if (token.hasMoreTokens())
			      weight = Integer.parseInt(token.nextToken());
			    else // No weight given -- set at 1
			      weight = 1;
			    G.setEdge(v1, v2, weight);
			    if (undirected) // Put in edge in other direction
			      G.setEdge(v2, v1, weight);
			  }
			  return G;
	}
	
	static void Gprint(ADTGraph G, StringBuffer out) {
		  int i, j;
		
		  for (i=0; i<G.n(); i++) {
		    for(j=0; j<G.n(); j++)
		      if (G.weight(i, j) == Integer.MAX_VALUE)
		        out.append("0 ");
		      else
		        out.append(G.weight(i, j) + " ");
		  }
	}
	
	
	
	
	/**
	 * output connected component;
	 * @param G
	 */
	static void concom(ADTGraph G) {
		  int i;
		  for (i=0; i<G.n(); i++)  // For n vertices in graph
		    G.setMark(i, 0);       // Vertex i in no component
		  
		  int comp = 1;            // Current component
		  
		  for (i=0; i<G.n(); i++)
		    if (G.getMark(i) == 0) // Start a new component
		      DFS_component(G, i, comp++);
		  
		  for (i=0; i<G.n(); i++)
		    out.append(i + "{" + G.getMark(i) + "} ");
	}
	
	
	static void DFS_component(ADTGraph G, int v, int comp) {
	  
	  G.setMark(v, comp);
	  
	  for (int w = G.first(v); w < G.n(); w = G.next(v, w))
	    if (G.getMark(w) == 0)
	      DFS_component(G, w, comp);
	}
	
	
	static void DFS(ADTGraph G, int v) { // Depth first search
		  PreVisit(G, v);                 // Take appropriate action
		  G.setMark(v, VISITED);
		  for (int w = G.first(v); w < G.n() ; w = G.next(v, w)) //fan in or fan out?
		    if (G.getMark(w) == UNVISITED)
		      DFS(G, w);
		  
		  
		  //PostVisit(G, v);                // Take appropriate action
		
  }
	
	
	static void DFS_nonrecursive(ADTGraph g, int v) {
		
		ADTStack<Integer> stack = new AStack<Integer>(g.n());
		stack.push(v);
		
		while (stack.length()!=0) {
	            int temp = stack.pop();
	            
	            if(g.getMark(temp)==UNVISITED) { 
	            	PreVisit(g, temp);
	            	g.setMark(temp, VISITED);
	            	
	            }
	            //for(int w=g.first(temp);w<g.n(); w=g.next(temp,w)){
	            for(int w=g.last(temp);w>=0; w=g.rnext(temp,w)){
	                if (g.getMark(w) == UNVISITED) {	                    
	                    stack.push(w);     
	                }
	            }
	            
	    }
	    
	   
		
	}
	
	
	
	static boolean hasCycle = false; 
	static int[] color ; //three colors for indicating white (0), gray(1) and black(2); 
	static  int last=-1;
	
	//define an algorithm on a graph. This is outside the graph class definition
	static void cycleDFS(Graph G, int v) {
		 
		  color[v] = 1; // set the node to gray; 
		 
		  PreVisit(G, v);                 // Take appropriate action
		 
		  for (int w = G.first(v); w < G.n() ; w = G.next(v, w)) {//checking all the fan out of the nodes; 
			if(color[w] == 1 && w != last) {
				hasCycle = true;
				break;
			}
			else if (color[w] == 0) {
			  last = v;  //recording where the fan in is from; 
		      cycleDFS(G, w);
		      
			}
		  
		  } 
		  
		  color[v] = 2; //after traversing all the fan-out, we set the color to black; 
	 
	}
	 
	
	
	static void DFS_syn(ADTGraph G, int v) { // Depth first search needs to complete visiting of all the fan-in nodes
		  PreVisit(G, v);              
		  G.setMark(v, VISITED);
		  for (int w = G.first(v); w < G.n() ; w = G.next(v, w))
		    if (G.getMark(w) == UNVISITED) {
		    	/*
		    	 * 
		    	 * if w's fan-in nodes is not visited, 
		    	 * vist the fan-in node; otherwise, call recursive function
		    	 * 
		    	 * The Graph needs to expend a function to return
		    	 * fan-in nodes of a given nodes; 
		    	 * 
		    	 */
		    	
		    /*	for (int w: G.getFanins()) {
		    		if (G.getMark(w) != VISITED) {
		    			PreVisit(w);
		    			G.setMark(w, VISITED);
		    		}
		    	}
		    	
		    	*/
		    	
		    	DFS_syn(G, w);
		    }
		  //PostVisit(G, v);     
		
	}
	
	/**
	 * depth first traversal
	 */
	   static void PreVisit(ADTGraph G, int v) {
		  out.append(v + "pre "); //subject to the application
		}

		static void PostVisit(ADTGraph G, int v) {
		  out.append(v + "pos "); // subject to the application 
		}

		
		
		
		
		 public boolean isCyclic(ADTGraph G, int start) {
			
			 ADTStack<Integer> stack = new AStack<Integer>(G.n());
				stack.push(start);
				while(stack.length()>0) {	
					int v = stack.topValue();
					stack.pop();
					G.setMark(v, VISITED);
					if(((Graphm) G).incrCount(v)>1)
						return true;
					int temp = v; 
					for (int w = G.first(temp); w < G.n(); w = G.next(temp,w)) {
					    if (G.getMark(w) == UNVISITED) { // Put next to stack   	
					    	stack.push(w);
					    	G.setMark(w, VISITED);  	
					    	int k = G.first(w); 
					    	while(k<G.n()) {
					    		 if (G.getMark(k) == UNVISITED) {
					    			 G.setMark(k, VISITED);
					    			 stack.push(k);
					    			 temp = k;
					    			 break;
					    		 }
					    		 k = G.next(w, k);
					    	}
					    }
					    if(((Graphm) G).incrCount(w)>1)
							return true;
					 }
				}
			 return false;
		 }
		
		/*
		static void DFS(Graph G, int start) {	
			
			ADTStack<Integer> stack = new AStack<Integer>(G.n());
			stack.push(start);
		
			while(stack.length()>0) {
				int v = stack.topValue();
				stack.pop();
				PreVisit(G, v);
				G.setMark(v, VISITED);
				int temp = v; 
				for (int w = G.first(temp); w < G.n(); w = G.next(temp,w)) {
				    if (G.getMark(w) == UNVISITED) { // Put next to stack
				    	
				    	stack.push(w);
				    	G.setMark(w, VISITED);
				    	
				    	int k = G.first(w); 
				    	while(k<G.n()) {
				    		 if (G.getMark(k) == UNVISITED) {
				    			 G.setMark(k, VISITED);
				    			 stack.push(k);
				    			 temp = k;
				    			 break;
				    		 }
				    		 k = G.next(w, k);
				    	}
				    	
				    	
				    }
				 }
				 PostVisit(G, v);
			}
		}
		
		*/
		void graphTraverse(Graph G) {
		  int v;
		  for (v=0; v<G.n(); v++)
		    G.setMark(v, UNVISITED); // Initialize 
		  for (v=0; v<G.n(); v++)
		    if (G.getMark(v) == UNVISITED)
		      DFS(G, v);
		}
		
		
		
		
		
		/**
		 *  Breadth first (queue-based) search
		 */
		static void BFS(Graph G, int start) {
		  ADTQueue<Integer> Q = new AQueue<Integer>(G.n());
		  Q.enqueue(start);
		  G.setMark(start, VISITED);
		  //PUT PRINTING HERE 
		  
		  
		  while (Q.length() > 0) {    // Process each vertex on Q
		    int v = Q.dequeue();
		    PreVisit(G, v);           // Take appropriate action
		    for (int w = G.first(v); w < G.n(); w = G.next(v, w))
		      if (G.getMark(w) == UNVISITED) { // Put neighbors on Q
		        G.setMark(w, VISITED);
		        Q.enqueue(w);
		      }
		    PostVisit(G, v);          //PUT PRINT HERE; 
		  }
		}
		
		
		static void printout(int v) {
			  out.append(v + " ");
		}
		/**
		 * topological sort with recursion 
		 */

		
		
		static void topsort(Graph G) { // Recursive topological sort
			  for (int i=0; i<G.n(); i++)  // Initialize Mark array
			    G.setMark(i, UNVISITED);
			  for (int i=0; i<G.n(); i++)  // Process all vertices
			    if (G.getMark(i) == UNVISITED)
			      tophelp(G, i);           // Recursive helper function
		}

		static void tophelp(Graph G, int v) {
			  G.setMark(v, VISITED);
			 
			  for (int w = G.first(v); w < G.n(); w = G.next(v, w))
			    if (G.getMark(w) == UNVISITED)
			      tophelp(G, w);
			  
			  printout(v);                 // PostVisit for Vertex v
		}
		
	
		/**
		 * topological sort using queue without recursion  
		 */
		static void topsortQueue(Graph G) { // Topological sort: Queue
			  ADTQueue<Integer> Q = new AQueue<Integer>(G.n());
			  int[] Count = new int[G.n()];
			  int v;
			  for (v=0; v<G.n(); v++) Count[v] = 0; // Initialize
			  
			  for (v=0; v<G.n(); v++)      // Process every edge
			    for (int w = G.first(v); w < G.n(); w = G.next(v, w))
			       Count[w]++;       // Add to v2's prereq count
			  	  
			  for (v=0; v<G.n(); v++)      // Initialize Queue
			    if (Count[v] == 0)         // V has no prerequisites
			      Q.enqueue(v);
			  
			  while (Q.length() > 0) {     // Process the vertices
			    v = Q.dequeue().intValue();
			    printout(v);               // PreVisit for Vertex V
			    for (int w = G.first(v); w < G.n(); w = G.next(v, w)) {
			      Count[w]--;              // One less prerequisite
			      if (Count[w] == 0)       // This vertex is now free
			        Q.enqueue(w);
			    }
			  }
			}
		
		
		
		  
		
	  /**
	   * This method is automatically called once before each test case
	   * method, so that all the variables are cleanly initialized for
	   * each test.
	   */
	  
	 static StringBuffer out;
	  
	 @BeforeEach
	  public void setUp()
	  {
	    out = new StringBuffer(100);
	  }
	
	  
	  
	 // @Test
	  public void testConnectComponent() throws IOException {
		    BufferedReader f;
		    f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-concomp.gph")));
		    Graph G = new Graphm();
		    createGraph(f, G);
		    concom(G);
		    assertEquals("0{1} 1{2} 2{2} 3{2} 4{1} 5{2} ",out.toString());
	  }
	  
	  @Test
	  public void testDFS() throws IOException {
			BufferedReader f;
			//f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-topsort.gph")));
			f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-dfs.gph")));
			//f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-spath-cycle.gph")));
			
			Graph G = new Graphm();
			createGraph(f, G);
			//DFS(G,0);
			DFS_nonrecursive(G, 0);
		
			assertEquals( "0pre 1pre 3pre 4pre 6pre 5pre 2pre ", out.toString());
	  }
	  
	  //@Test
	  public void testGetFanIn() throws IOException {
		  	BufferedReader f;
			//f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-traversal.gph")));
			f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-dfs.gph")));
			Graph G = new Graphm();
			createGraph(f, G);
			
			int v = 0;
			Object[] fanIn = G.getFanIn(v);
			if(fanIn!=null) {
				for(Object item:fanIn) {
					out.append(item.toString()+"; ");
				}
				assertEquals("1; 2; ", out.toString());
			}
			else
				assertTrue("No fan-in for node "+v, true);
	  }
	  
	  
	  //@Test
	  public void testBFS() throws IOException {
			BufferedReader f;
			f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-traversal.gph")));
			Graph G = new Graphm();
			createGraph(f, G);
			BFS(G, 0);
		    assertEquals(out.toString(), "0 0 2 2 4 4 1 1 3 3 5 5 ");
			    
	  }
	  
	 // @Test
	  public void testTopSort() throws IOException {
			BufferedReader f;
			f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-topsort.gph")));
			//f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-dfs.gph")));
			Graph G = new Graphm();
			createGraph(f, G);
			topsort(G);
			assertEquals( "6 4 3 5 1 2 0 ", out.toString());
	 }
	  
	 //@Test
	  public void testTopSortQueue() throws IOException {
			BufferedReader f;
			f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-topsort.gph")));
			//f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-dfs.gph")));
			Graph G = new Graphm();
			createGraph(f, G);
			topsortQueue(G);
			assertEquals( "0 1 2 5 3 4 6 ", out.toString());
	}
	  
	
	
	  
	  @Test
	  public void testCycle() throws IOException{
				BufferedReader f;
				f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-cycle.gph")));
				Graph G = new Graphm();
				createGraph(f, G);
				
				color = new int[G.n()];
				for(int i =0;i<color.length;i++)
					 color[i] = 0;  // colors of all nodes are white; 
				 
				int last = -1; // to judge if the last visited fan out node is gray; 
				hasCycle = false;
				
				System.out.println(out.toString());
				cycleDFS(G,0);
				assertTrue(hasCycle);
				
				
				//assertEquals( false, isCyclic(G, 0));
				
		}
	  
	

}