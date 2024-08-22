package coen352.ch11.graph.shortestpath;

import static org.junit.jupiter.api.Assertions.*;

import java.io.*;
import java.util.StringTokenizer;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

import coen352.ch4.list.*;
import coen352.ch4.queue.*;

class ShortestPathTest {


		static final int UNVISITED = 0;
		static final int VISITED = 1;
		
		// Create a graph from file
		static Graph createGraph(BufferedReader file, Graph G) 
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
		
		static void Gprint(Graph G, StringBuffer out) {
			  int i, j;
			
			  for (i=0; i<G.n(); i++) {
			    for(j=0; j<G.n(); j++)
			      if (G.weight(i, j) == Integer.MAX_VALUE)
			        out.append("0 ");
			      else
			        out.append(G.weight(i, j) + " ");
			  }
		}
		

			
		static int[] D = new int[100];
		static StringBuffer out,path;
		
		
		static int[][] pMatrix;
		
		static void AddEdgetoShortestPath(int v1, int v2) {
			path.append("(" + v1 + "," + v2 + ") "); 
		}
		
		static void AddVertextoShortestPath(int v) {
			path.append(  v + "-> "); 
		}

		/**
		 * Simple Source Shortest Path
		 * @param G
		 * @param D
		 * @return
		 */
		static int minVertex(Graph G, int[] D) {
			  int v = 0;  // Initialize v to any unvisited vertex;
			  for (int i=0; i<G.n(); i++)
			    if (G.getMark(i) == UNVISITED) { v = i; break; }
			  
			  for (int i=0; i<G.n(); i++)  // Now find smallest value
			    if ((G.getMark(i) == UNVISITED) && (D[i] < D[v]))
			      v = i;
			  return v;
		}
		
		// Compute shortest path distances from s, store them in D
		static void Dijkstra(Graph G, int s, int[] D) {
		  // LList[] pList = new LList[G.n()];
		  // for(int i=0;i<G.n();i++) {
		  //	  pList[i] = new LList<Integer>();
		  // }
			 
		  for (int i=0; i<G.n(); i++)    // Initialize
		    D[i] = Integer.MAX_VALUE;
		  
		  D[s] = 0;
		  
		  int[] pList = new int[G.n()];
		  for(int i=0;i<G.n();i++) {
			pList[i] = Integer.MAX_VALUE;
		  }
		  
		  
		  for (int i=0; i<G.n(); i++) {  // Process the vertices
		    
			int v = minVertex(G, D);     // Find next-closest vertex
			
			
			
			
			G.setMark(v, VISITED);
		    if (D[v] == Integer.MAX_VALUE) return; // Unreachable
		    
		    AddVertextoShortestPath(v);
		    
		    int minDist = Integer.MAX_VALUE;
		    for (int w = G.first(v); w < G.n(); w = G.next(v, w)) {
		      
		      if(minDist > G.weight(v, w)) {
		        	minDist = G.weight(v, w);
		        	pList[v] = w;
		      }
		      if (D[w] > (D[v] + G.weight(v, w))) {
		        D[w] = D[v] + G.weight(v, w); 
		       
		      } 
		      
		    }
		   	
		  }
		}
		
		
		
		// Dijkstra's shortest-paths: priority queue version
		static void DijkstraMinHeap(Graph G, int s, int[] D) {
		  int v;                              // The current vertex
		  DijkElem[] E = new DijkElem[G.e()]; // Heap for edges
		  E[0] = new DijkElem(s, 0);          // Initial vertex
		  
		  MinHeap<DijkElem> H = new MinHeap<DijkElem>(E, 1, G.e());
		  for (int i=0; i<G.n(); i++)         // Initialize distance
		    D[i] = Integer.MAX_VALUE;
		  
		  D[s] = 0;
		  for (int i=0; i<G.n(); i++) {       // For each vertex
			  
		    if(H.heapsize()>0) {
		    	do { v = (H.removemin()).vertex(); }    // Get position
		    	while (G.getMark(v) == VISITED);
		    	G.setMark(v, VISITED);
				
		    	AddVertextoShortestPath(v);
		    
			    if (D[v] == Integer.MAX_VALUE) return;  // Unreachable
			    for (int w = G.first(v); w < G.n(); w = G.next(v, w))
			      if (D[w] > (D[v] + G.weight(v, w))) { // Update D
			        D[w] = D[v] + G.weight(v, w);
			        H.insert(new DijkElem(w, D[w]));      
			     }
		    }
		  }
		}
		
		  @BeforeEach
		  public void setUp()
		  {
		    out = new StringBuffer(100);
		    path = new StringBuffer(250);
		  }
		
		  
		  
		  @Test
		  public void testSimpleShortestPath()
				  throws IOException {
				    BufferedReader f;
				    f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-spath.gph")));
				    Graph G = new Graphm();
				    createGraph(f, G);
				    Dijkstra(G, 2, D);
				    
				    System.out.println("testSimpleShortestPath :" +path);
				    //assertEquals(path, "0-> 2-> 1-> 3-> 4->");
				    for (int i=0; i< G.n(); i++) out.append(D[i] + " ");
				    assertEquals(out.toString(), "0 5 3 10 18 ");
				    
				    
		  }
		  
//		  @Test
//		  public void testMinHeapShortestPath()
//				  throws IOException {
//				    BufferedReader f;
//				    f = new BufferedReader(new InputStreamReader(new FileInputStream("testfile-spath.gph")));
//				    Graph G = new Graphm();
//				    createGraph(f, G);
//				    DijkstraMinHeap(G,2, D);
//				    
//				    System.out.println("testMinHeapShortestPath:" +path);
//				    
//				    for (int i=0; i< G.n(); i++) out.append(D[i] + " ");
//				    assertEquals(out.toString(), "0 5 3 10 18 ");
//		  }
//		  
		  @Test
		  public void testMinHeapShortestPath()
				  throws IOException {
				    BufferedReader f;
				    f = new BufferedReader(new InputStreamReader
				    		(new FileInputStream("testfile-spath.gph")));
				    Graph G = new Graphm();
				    createGraph(f, G);
				    
				    DijkstraMinHeap(G,0, D); 
				    assertEquals(path, "0 5 3 10 18 ");
				    // write the assertion statement 
				    
		   }
		  
		  

	}