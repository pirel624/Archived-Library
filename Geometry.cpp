namespace geometry
{
    class point
    {
        public:
        float x = 0;
        float y = 0;
    };

    class line
    {
        public:
        float slope = 1;
        float constant = 1;
        void CreateFromPoints(point point1, point point2)
        {
            if((point2.x - point1.x) == 0)
            {
                slope = (point2.y - point1.y) / 0.001f;
            }
            else
            {
                slope = (point2.y - point1.y) / (point2.x - point1.x);
            }

            constant = point1.y - point1.x * slope;
        }
    };

    class segment
    {
        private:
        point limit1;
        point limit2;
        line base_line;

        public:

        void ChangeLimit1(float x, float y)
        {
            limit1.x = x;
            limit1.y = y;
            base_line.CreateFromPoints(limit1, limit2);
        }

        void ChangeLimit2(float x, float y)
        {
            limit2.x = x;
            limit2.y = y;
            base_line.CreateFromPoints(limit1, limit2);
        }

        float GetSlope()
        {
            return base_line.slope;
        }

        float GetConstant()
        {
            return base_line.constant;
        }

        point GetLimit1()
        {
            return limit1;
        }

        point GetLimit2()
        {
            return limit2;
        }

        line GetBaseLine()
        {
            return base_line;
        }
    };

    class circle
    {
        public:
        point center;
        float radius;
    };


    void LineIntersection(line line1, line line2, point &location, bool &IsIntersecting)
    {
        if(line1.slope == line2.slope)
        {
            IsIntersecting = false;
        }
        else
        {
            location.x = (line2.constant - line1.constant) / (line1.slope - line2.slope);
            location.y = (location.x * line1.slope) + line1.constant;
            IsIntersecting = true;
        }
    }

    void SegmentIntersection(segment segment1, segment segment2, point &location, bool &IsIntersecting)
    {
        point position;
        bool intersecting;
        LineIntersection(segment1.GetBaseLine(), segment2.GetBaseLine(), position, intersecting);
        if(!intersecting)
        {
            IsIntersecting = false;
        }
        else
        {
            bool check1, check2, check3, check4 = false;

            // Segment1

            if(segment1.GetLimit1().x <= segment1.GetLimit2().x) // x1 >> x2
            {
                if((position.x >= segment1.GetLimit1().x) && (position.x <= segment1.GetLimit2().x)){check1 = true;}
            }
            else                                                 // x2 >> x1
            {
                if((position.x <= segment1.GetLimit1().x) && (position.x >= segment1.GetLimit2().x)){check1 = true;}
            }

            if(segment1.GetLimit1().y <= segment1.GetLimit2().y) // y1 >> y2
            {
                if((position.y >= segment1.GetLimit1().y) && (position.y <= segment1.GetLimit2().y)){check2 = true;}
            }
            else                                                 // y2 >> y1
            {
                if((position.y <= segment1.GetLimit1().y) && (position.y >= segment1.GetLimit2().y)){check2 = true;}
            }

            // Segment 2

            if(segment2.GetLimit1().x <= segment2.GetLimit2().x) // x1 >> x2
            {
                if((position.x >= segment2.GetLimit1().x) && (position.x <= segment2.GetLimit2().x)){check3 = true;}
            }
            else                                                 // x2 >> x1
            {
                if((position.x <= segment2.GetLimit1().x) && (position.x >= segment2.GetLimit2().x)){check3 = true;}
            }

            if(segment2.GetLimit1().y <= segment2.GetLimit2().y) // y1 >> y2
            {
                if((position.y >= segment2.GetLimit1().y) && (position.y <= segment2.GetLimit2().y)){check4 = true;}
            }
            else                                                 // y2 >> y1
            {
                if((position.y <= segment2.GetLimit1().y) && (position.y >= segment2.GetLimit2().y)){check4 = true;}
            }

            if(check1 && check2 && check3 && check4)
            {
                IsIntersecting = true;
                location = position;
            }
            else
            {
                IsIntersecting = false;
                location = position;
            }
        }
    }

    bool LineCircleCollision(line line1, circle circle1)
    {
        float A = circle1.center.x;
        float B = circle1.center.y;
        float R = circle1.radius;
        float M1 = line1.slope;
        float C1 = line1.constant;

        float M2 = -1.0f / M1;
        float C2 = B - A * M2;

        line garis1;
        garis1.slope = M1;
        garis1.constant = C1;

        line garis2;
        garis2.slope = M2;
        garis2.constant = C2;

        point intersection;
        bool intersecting;      // this variable is actually useless, i instantiate it because the following function need a boolean buffer
        LineIntersection(garis1, garis2, intersection, intersecting);

        float distance = std::sqrt((intersection.x - A) * (intersection.x - A) + (intersection.y - B) * (intersection.y - B));
        
        if(distance <= R){return true;}
        else{return false;}
    }

    void LineCircleIntersection(line line1, circle circle1, point &intersection1, point &intersection2, bool &IsIntersecting)
    {
        float A = circle1.center.x;
        float B = circle1.center.y;
        float R = circle1.radius;
        float M1 = line1.slope;
        if(M1 == 0){M1 = 0.001f;}
        float C1 = line1.constant;

        float M2 = -1.0f / M1;
        float C2 = B - A * M2;

        line garis1;
        garis1.slope = M1;
        garis1.constant = C1;

        line garis2;
        garis2.slope = M2;
        garis2.constant = C2;

        point intersection;
        bool intersecting;      // this variable is actually useless, i instantiate it because the following function need a boolean buffer
        LineIntersection(garis1, garis2, intersection, intersecting);

        float distance = std::sqrt((intersection.x - A) * (intersection.x - A) + (intersection.y - B) * (intersection.y - B));

        if(distance <= R)
        {
            IsIntersecting = true;

            float interpolation_length = std::sqrt(R * R - distance * distance);
            float dx = std::sqrt(interpolation_length * interpolation_length / (M1 * M1 + 1));
            float dy = dx * M1;

            //intersection1.x = interpolation_length;

            
            if(M1 > 0)
            {
                intersection1.x = intersection.x + dx;
                intersection1.y = intersection.y + dy;

                intersection2.x = intersection.x - dx;
                intersection2.y = intersection.y - dy;
            }
            else if(M1 < 0)
            {
                intersection1.x = intersection.x + dx;   // i dont know why, but it just works
                intersection1.y = intersection.y + dy;

                intersection2.x = intersection.x - dx;
                intersection2.y = intersection.y - dy;
            }
            else if(M1 == 0)
            {
                intersection1.x = intersection.x + dx;
                intersection1.y = intersection.y;

                intersection2.x = intersection.x - dx;
                intersection2.y = intersection.y;
            }
            
        }
        else
        {
            point buffer; buffer.x = 1; buffer.y = 1;
            IsIntersecting = false;
        }
    }
}
