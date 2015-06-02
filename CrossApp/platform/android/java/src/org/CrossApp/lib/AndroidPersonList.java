package org.CrossApp.lib;
import java.util.ArrayList;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import android.app.Activity;
import android.database.Cursor;
import android.provider.ContactsContract;
import android.provider.ContactsContract.CommonDataKinds.Nickname;

import android.provider.ContactsContract.Data;

public class AndroidPersonList
{
	private static Activity s_pContext;
	public static void Init( final Activity context )
	{
		s_pContext = context;
	}
	public static class FriendData
	{
		public String name;
		public ArrayList<String> phoneNumber = new ArrayList<String>();
		public String emailValue;
		public String address_street;  
        public String address_city;  
        public String address_region;  
        public String address_postCode;  
        public String address_formatAddress;
		public String nickname;
	}
	public static String CAGetPersonList()
	{
		// ������е���ϵ��  
        Cursor cur = s_pContext.getContentResolver().query(  
                ContactsContract.Contacts.CONTENT_URI,  
                null,  
                null,  
                null,  
                ContactsContract.Contacts.DISPLAY_NAME  
                        + " COLLATE LOCALIZED ASC");  
        ArrayList<FriendData> vecFriend = new ArrayList<FriendData>();
        // ѭ������  
        if (cur.moveToFirst())
        {
            int idColumn = cur.getColumnIndex(ContactsContract.Contacts._ID);  
  
            int displayNameColumn = cur  
                    .getColumnIndex(ContactsContract.Contacts.DISPLAY_NAME);  
  
       	    do 
       	    {
       	    	try
				{
	            	FriendData data = new FriendData();
	                // �����ϵ�˵�ID��  
	                String contactId = cur.getString(idColumn);  
	                // �����ϵ������  
	                String disPlayName = cur.getString(displayNameColumn);  
	                data.name = disPlayName;
	                int phoneCount = cur  
	                        .getInt(cur  
	                                .getColumnIndex(ContactsContract.Contacts.HAS_PHONE_NUMBER));  
	                if (phoneCount > 0)
	                {  
	                    Cursor phones = s_pContext.getContentResolver().query(  
	                            ContactsContract.CommonDataKinds.Phone.CONTENT_URI,  
	                            null,  
	                            ContactsContract.CommonDataKinds.Phone.CONTACT_ID  
	                                    + " = " + contactId, null, null);  
	                    if (phones.moveToFirst())
	                    {  
	                        do
	                        {   
	                            String phoneNumber = phones  
	                                    .getString(phones  
	                                            .getColumnIndex(ContactsContract.CommonDataKinds.Phone.NUMBER));   
	                            //Log.i("phoneNumber", phoneNumber);  
	                            //Log.i("phoneType", phoneType);  
	                            data.phoneNumber.add( phoneNumber );
	                        }
	                        while (phones.moveToNext());  
	                    }
	                    phones.close();
	                }  
	  
	                // ��ȡ����ϵ������  
	                Cursor emails = s_pContext.getContentResolver().query(  
	                        ContactsContract.CommonDataKinds.Email.CONTENT_URI,  
	                        null,  
	                        ContactsContract.CommonDataKinds.Phone.CONTACT_ID  
	                                + " = " + contactId, null, null);  
	                if (emails.moveToFirst())
	                {  
	                    do
	                    {  
	                        // �������еĵ绰����    
	                        String emailValue = emails  
	                                .getString(emails  
	                                        .getColumnIndex(ContactsContract.CommonDataKinds.Email.DATA));  
	                          
	                        //Log.i("emailType", emailType);  
	                        //Log.i("emailValue", emailValue);  
	                        data.emailValue = emailValue;
	                    }
	                    while (emails.moveToNext());  
	                }  
	                emails.close();
	                 //��ȡ����ϵ�˵�ַ  
	                Cursor address = s_pContext.getContentResolver()  
	                        .query(  
	                                ContactsContract.CommonDataKinds.StructuredPostal.CONTENT_URI,  
	                                null,  
	                                ContactsContract.CommonDataKinds.Phone.CONTACT_ID  
	                                        + " = " + contactId, null, null);  
	                if (address.moveToFirst()) 
	                {  
	                    do {  
	                        // �������еĵ�ַ  
	                        String street = address  
	                                .getString(address  
	                                        .getColumnIndex(ContactsContract.CommonDataKinds.StructuredPostal.STREET));  
	                        String city = address  
	                                .getString(address  
	                                        .getColumnIndex(ContactsContract.CommonDataKinds.StructuredPostal.CITY));  
	                        String region = address  
	                                .getString(address  
	                                        .getColumnIndex(ContactsContract.CommonDataKinds.StructuredPostal.REGION));  
	                        String postCode = address  
	                                .getString(address  
	                                        .getColumnIndex(ContactsContract.CommonDataKinds.StructuredPostal.POSTCODE));  
	                        String formatAddress = address  
	                                .getString(address  
	                                        .getColumnIndex(ContactsContract.CommonDataKinds.StructuredPostal.FORMATTED_ADDRESS));
	                        data.address_street = street;  
	                        data.address_city = city;  
	                        data.address_region = region;  
	                        data.address_postCode = postCode;  
	                        data.address_formatAddress = formatAddress;
	                    } 
	                    while (address.moveToNext());  
	                }  
	                address.close();
	                 //��ȡnickname��Ϣ  
	                Cursor nicknames = s_pContext.getContentResolver().query(  
	                        Data.CONTENT_URI,  
	                        new String[] { Data._ID, Nickname.NAME },  
	                        Data.CONTACT_ID + "=?" + " AND " + Data.MIMETYPE + "='"  
	                                + Nickname.CONTENT_ITEM_TYPE + "'",  
	                        new String[] { contactId }, null);  
	                if (nicknames.moveToFirst()) 
	                {  
	                    do 
	                    {  
	                        String nickname_ = nicknames.getString(nicknames  
	                                .getColumnIndex(Nickname.NAME));  
	                        //Log.i("nickname_", nickname_);  
	                        data.nickname = nickname_;
	                    } 
	                    while (nicknames.moveToNext());  
	                } 
	                nicknames.close();
	                if ( data.name != null )
	                	vecFriend.add(data);
				}
	            catch( Exception e )
	            {
	            	
	            }
            }
        	while (cur.moveToNext());  
        }
        
        try
        {  
            // �����������{}���Ǵ���һ������  
            JSONObject personList = new JSONObject();
            JSONArray personArray = new JSONArray();
            for ( int i = 0 ; i < vecFriend.size(); i ++ )
            {
            	FriendData data = (FriendData)vecFriend.get(i);
	            JSONObject person = new JSONObject();
	            person.put("name", data.name);
	            person.put("address_street" , data.address_street != null ? data.address_street : "null" );  
	            person.put("address_city" , data.address_city != null ? data.address_city : "null" );  
	            person.put("address_region" , data.address_region != null ? data.address_region : "null" );  
	            person.put("address_postCode" , data.address_postCode != null ? data.address_postCode : "null" );  
	            person.put("address_formatAddress" , data.address_formatAddress != null ? data.address_formatAddress : "null" );
	    		person.put("nickname" , data.nickname != null ? data.nickname : "null" );
	            // ��һ����phone��ֵ�����飬������Ҫ�����������  
	            JSONArray phone = new JSONArray();
	            for ( int j = 0 ; j < data.phoneNumber.size(); j ++ )
	            {
	            	phone.put((String)data.phoneNumber.get(j));
	            }
	            person.put("phone", phone);  
	            if ( data.emailValue != null )
	            	person.put("email", data.emailValue);      
	            personArray.put(person);
            }
            personList.put("person", personArray);
            String ret = personList.toString();
            return ret;
        } 
        catch (JSONException ex) 
        {  
            // ��Ϊnull��ʹ��json��֧�ֵ����ָ�ʽ(NaN, infinities)  
            throw new RuntimeException(ex);  
        }
	}
}
