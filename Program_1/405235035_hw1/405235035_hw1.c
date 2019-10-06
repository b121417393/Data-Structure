#include <stdio.h>
#include <stdlib.h>

struct node
{
    int number;
    double x_axis;
    double y_axis;
};
typedef struct node Node;

struct edge
{
    int valid;
    int number;
    int beginning;
    int destination;
};
typedef struct edge Edge;


int main(int argc, char *argv[])
{
    int points_num=0 , edges_num=0;
    int i=0,j=0,k=0;
    double distance=0 ,radius = 0;
    Node points[1024],center;
    Edge edges[2048];
    FILE *fp_node , *fp_link;

    fp_node = fopen("node.txt", "r+");

    fscanf(fp_node,"%d",&points_num);
    
    //Take the data from node.txt
    for(i=0;i<points_num;i++)
    {
        fscanf(fp_node,"%d",&points[i].number);
        fscanf(fp_node,"%lf",&points[i].x_axis);
        fscanf(fp_node,"%lf",&points[i].y_axis);
    }

    fclose(fp_node);

    //Determine if there is a link between two nodes
    for(i=0;i<points_num;i++)
        for(j=i+1;j<points_num;j++)
        {
            distance=(points[i].x_axis-points[j].x_axis)*(points[i].x_axis-points[j].x_axis)+(points[i].y_axis-points[j].y_axis)*(points[i].y_axis-points[j].y_axis);
            if(distance<=1)
            {
                edges[k].valid=1;
                edges[k].number=k;
                edges[k].beginning=i;
                edges[k].destination=j;
                k++;
                edges_num++;
            }
        }

    fp_link = fopen("link.txt", "w+");

    fprintf( fp_link , "%d\n" , edges_num);
    
    for(i=0;i<edges_num;i++)
        fprintf( fp_link , "%d\t%d\t%d\n" , edges[i].number,edges[i].beginning,edges[i].destination);
    
    //Remove links with crosses
    for(i=0;i<k;i++)
    {
        //Find the center point of the link
        center.x_axis=(points[edges[i].beginning].x_axis+points[edges[i].destination].x_axis)/2;
        center.y_axis=(points[edges[i].beginning].y_axis+points[edges[i].destination].y_axis)/2;
        //The square of this link radius
        radius=(points[edges[i].beginning].x_axis-center.x_axis)*(points[edges[i].beginning].x_axis-center.x_axis)+(points[edges[i].beginning].y_axis-center.y_axis)*(points[edges[i].beginning].y_axis-center.y_axis);

        //Compare whether there are nodes in this circle
        for(j=0;j<points_num;j++)
        {
            if(points[j].number==edges[i].beginning || points[j].number==edges[i].destination)
                continue;
            distance=(center.x_axis-points[j].x_axis)*(center.x_axis-points[j].x_axis)+(center.y_axis-points[j].y_axis)*(center.y_axis-points[j].y_axis);
            if(distance<radius)
            {
                edges[i].valid=0;
                edges_num--;
                break;
            }
        }
    }


    fprintf( fp_link , "%d\n" , edges_num);

    for(i=0;i<k;i++)
        if(edges[i].valid==1)
            fprintf( fp_link , "%d\t%d\t%d\n" , edges[i].number,edges[i].beginning,edges[i].destination);

    fclose(fp_link);

    return 0;
}
